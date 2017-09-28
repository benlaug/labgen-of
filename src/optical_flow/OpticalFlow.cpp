/**
 * Copyright - Benjamin Laugraud <blaugraud@ulg.ac.be> - 2017
 * http://www.montefiore.ulg.ac.be/~blaugraud
 * http://www.telecom.ulg.ac.be/labgen
 *
 * This file is part of LaBGen-OF.
 *
 * LaBGen-OF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LaBGen-OF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LaBGen-OF.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cmath>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/optflow.hpp>

#include <labgen-of/optical_flow/OpticalFlow.hpp>
#include <labgen-of/Utils.hpp>

using namespace std;
using namespace boost;
using namespace cv;
using namespace cv::optflow;
using namespace ns_labgen_of;

/* ========================================================================== *
 * OpticalFlow                                                                *
 * ========================================================================== */

void OpticalFlow::read_opt_file(
  const string& opt_path,
  Mat& optical_flow
) {
  if (!filesystem::exists(opt_path)) {
    throw runtime_error(
      "The optical flow file '" + opt_path + "' does not exist"
    );
  }

  optical_flow = readOpticalFlow(opt_path);
}

/******************************************************************************/

void OpticalFlow::write_opt_file(
  const string& opt_path,
  const Mat& optical_flow
) {
  writeOpticalFlow(opt_path, optical_flow);
}

/******************************************************************************/

void OpticalFlow::get_l2_norms(
  const Mat& optical_flow,
  Mat& l2_norms
) {
  check_optical_flow(optical_flow);
  float* optical_flow_buffer = (float*)optical_flow.data;

  int32_t height = optical_flow.rows;
  int32_t width = optical_flow.cols;

  if (l2_norms.empty())
    l2_norms = cv::Mat(height, width, CV_64FC1);
  else if (l2_norms.type() != CV_64FC1)
    throw logic_error("The l2-norms must be in a 1-channel matrix of doubles");

  double* l2_norms_buffer = (double*)l2_norms.data;

  for (int32_t i = 0; i < height * width; ++i, optical_flow_buffer += 2) {
    *(l2_norms_buffer++) = sqrt(
      (static_cast<double>(optical_flow_buffer[0]) * optical_flow_buffer[0]) +
      (static_cast<double>(optical_flow_buffer[1]) * optical_flow_buffer[1])
    );
  }
}

/******************************************************************************/

void OpticalFlow::get_angles(
  const Mat& optical_flow,
  Mat& angles,
  bool degrees
) {
  check_optical_flow(optical_flow);
  float* optical_flow_buffer = (float*)optical_flow.data;

  int32_t height = optical_flow.rows;
  int32_t width = optical_flow.cols;

  if (angles.empty())
    angles = cv::Mat(height, width, CV_64FC1);
  else if (angles.type() != CV_64FC1)
    throw logic_error("The angles must be in a 1-channel matrix of doubles");

  double* angles_buffer = (double*)angles.data;

  for (int32_t i = 0; i < height * width; ++i, optical_flow_buffer += 2) {
    *(angles_buffer++) = atan2(
      static_cast<double>(optical_flow_buffer[1]),
      static_cast<double>(optical_flow_buffer[0])
    ) * (degrees ? (180. / M_PI) : 1);
  }
}

/******************************************************************************/

void OpticalFlow::get_color_image(
  const Mat& optical_flow,
  Mat& color_image,
  bool inverse_s_and_v
) {
  check_optical_flow(optical_flow);

  /*
   * Taken and modified from http://stackoverflow.com/questions/7693561/opencv-
   * displaying-a-2-channel-image-optical-flow
   */

  /* Separate x and y direction vectors. */
  Mat xy[2];
  split(optical_flow, xy);

  /* Calculate angle and magnitude. */
  Mat magnitude, angle;
  cartToPolar(xy[0], xy[1], magnitude, angle, true);

  /* Translate magnitude to range [0, 1]. */
  Utils::normalize_mat(magnitude);

  /* Build HSV image. */
  Mat _hsv[3], hsv;
  _hsv[0] = angle;
  _hsv[inverse_s_and_v ? 2 : 1] = magnitude;
  _hsv[inverse_s_and_v ? 1 : 2] = Mat::ones(angle.size(), CV_32F);
  merge(_hsv, 3, hsv);

  /* Convert to BGR. */
  cvtColor(hsv, color_image, COLOR_HSV2BGR);
  color_image *= 255;
  color_image.convertTo(color_image, CV_8U);
}

/******************************************************************************/

void OpticalFlow::get_arrows_image(
  const Mat& input_frame,
  const Mat& optical_flow,
  Mat& arrows_image,
  size_t step_px,
  double visibility_factor
) {
  check_optical_flow(optical_flow);
  input_frame.copyTo(arrows_image);

  const int height = input_frame.rows;
  const int width = input_frame.cols;

  float* optical_flow_buffer = reinterpret_cast<float*>(optical_flow.data);

  for (int row = step_px - 1; row <= height - step_px; row += step_px) {
    for (int col = step_px - 1; col <= width - step_px; col += step_px) {
      arrowedLine(
        arrows_image,
        Point(col, row),
        Point(
          col + optical_flow_buffer[row*width*2 + col*2  ] * visibility_factor,
          row + optical_flow_buffer[row*width*2 + col*2+1] * visibility_factor
        ),
        Scalar(0, 0, 255)
      );
    }
  }
}

/******************************************************************************/

void OpticalFlow::check_optical_flow(const Mat& optical_flow) {
  if (optical_flow.type() != CV_32FC2)
    throw logic_error("The optical flow must be a 2-channels matrix of floats");
}

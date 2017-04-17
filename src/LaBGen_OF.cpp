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
#include <algorithm>
#include <stdexcept>

#include <opencv2/imgproc/imgproc.hpp>

#include <labgen-of/optical_flow/OpticalFlowFactory.hpp>
#include <labgen-of/LaBGen_OF.hpp>
#include <labgen-of/Utils.hpp>

using namespace std;
using namespace cv;
using namespace ns_labgen_of;
using namespace ns_labgen_of::ns_internals;

/* ========================================================================== *
 * LaBGen_OF                                                             *
 * ========================================================================== */

LaBGen_OF::LaBGen_OF(
  size_t height,
  size_t width,
  string a,
  int32_t s,
  int32_t n,
  int32_t p,
  double t,
  const Mat& first_frame
) :
height(height),
width(width),
a(a),
s(s),
n(n),
p(p),
t(t),
previous_frame(),
_previous_frame(),
optical_flow(OpticalFlowFactory::get_optical_flow(a)),
segmentation_map(Mat(height, width, CV_8UC1)),
history(Utils::getROIs(height, width, n), s) {
  first_frame.copyTo(previous_frame);

  if (optical_flow->get_frame_type() == CV_8UC1)
    cvtColor(first_frame, _previous_frame, CV_BGR2GRAY);
  else
    _previous_frame = previous_frame;
}

/******************************************************************************/

void LaBGen_OF::insert(const Mat& current_frame) {
  /* Convert frame if necessary. */
  Mat _current_frame;

  if (optical_flow->get_frame_type() == CV_8UC1)
    cvtColor(current_frame, _current_frame, CV_BGR2GRAY);
  else
    _current_frame = current_frame;

  /* Compute optical flow. */
  optical_flow->compute_flow(_previous_frame, _current_frame, vector_field);

  /* Compute l2-norms of the vectors. */
  OpticalFlow::get_l2_norms(vector_field, vector_field_l2);

  /* Spatial normalization of the l2-norms. */
  Utils::normalize_mat(vector_field_l2);

  /* Threshold spatially normalized l2-norms to produce segmentation map. */
  threshold_magnitude(vector_field_l2, segmentation_map, t);

  /* Insert the previous frame along with the segmentation map into the
   * history.
   */
  history.insert(segmentation_map, previous_frame);

  /* Keep the current frame as the previous one. */
  current_frame.copyTo(previous_frame);

  if (optical_flow->get_frame_type() == CV_8UC1)
    _previous_frame = _current_frame;
  else
    _previous_frame = previous_frame;
}

/******************************************************************************/

void LaBGen_OF::generate_background(Mat& background) const {
  if (history.empty()) {
    throw runtime_error(
      "Cannot generate the background with less than two inserted frames"
    );
  }

  history.median(background, s);
}

/******************************************************************************/

size_t LaBGen_OF::get_height() const {
  return height;
}

/******************************************************************************/

size_t LaBGen_OF::get_width() const {
  return width;
}

/******************************************************************************/

string LaBGen_OF::get_a() const {
  return a;
}

/******************************************************************************/

int32_t LaBGen_OF::get_s() const {
  return s;
}

/******************************************************************************/

int32_t LaBGen_OF::get_n() const {
  return n;
}

/******************************************************************************/

int32_t LaBGen_OF::get_p() const {
  return p;
}

/******************************************************************************/

double LaBGen_OF::get_t() const {
  return t;
}

/******************************************************************************/

const Mat& LaBGen_OF::get_vector_field() const {
  return vector_field;
}

/******************************************************************************/

const Mat& LaBGen_OF::get_normalized_magnitudes() const {
  return vector_field_l2;
}

/******************************************************************************/

const Mat& LaBGen_OF::get_segmentation_map() const {
  return segmentation_map;
}

/******************************************************************************/

void LaBGen_OF::threshold_magnitude(
  const Mat& l2_norms,
  Mat& result,
  double threshold
) {
  if (result.empty())
    result = Mat(l2_norms.rows, l2_norms.cols, CV_8UC1);

  double* data = reinterpret_cast<double*>(l2_norms.data);
  unsigned char* results_buffer = result.data;
  size_t num_pixels = l2_norms.total();

  for (size_t i = 0; i < num_pixels; ++i)
    *(results_buffer++) = (*(data++) > threshold) ? 255 : 0;
}

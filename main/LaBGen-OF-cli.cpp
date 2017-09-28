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
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <labgen-of/ArgumentsHandler.hpp>
#include <labgen-of/LaBGen_OF.hpp>
#include <labgen-of/GridWindow.hpp>
#include <labgen-of/TextProperties.hpp>

using namespace boost;
using namespace cv;
using namespace std;
using namespace ns_labgen_of;

/******************************************************************************
 * Main program                                                               *
 ******************************************************************************/

int main(int argc, char** argv) {
  /****************************************************************************
   * Argument(s) handling.                                                    *
   ****************************************************************************/

  /*
   * Extract parameters and sanity check.
   */

  ArgumentsHandler args_h(argc, argv);

  if (args_h.ask_for_help()) {
    args_h.print_help();
    return EXIT_SUCCESS;
  }

  /*
   * Welcome message.
   */

  cout << "===========================================================" << endl;
  cout << "= LaBGen-OF                                               =" << endl;
  cout << "===========================================================" << endl;
  cout << "= Copyright - Benjamin Laugraud - 2017                    =" << endl;
  cout << "= http://www.montefiore.ulg.ac.be/~blaugraud              =" << endl;
  cout << "= http://www.telecom.ulg.ac.be/labgen                     =" << endl;
  cout << "===========================================================" << endl;
  cout << endl;

  args_h.parse_vars_map();
  args_h.print_parameters();

  /****************************************************************************
   * Reading sequence.                                                        *
   ****************************************************************************/

  VideoCapture decoder(args_h.get_input());

  if (!decoder.isOpened()) {
    throw runtime_error(
      "Cannot open the '" + args_h.get_input() + "' sequence."
    );
  }

  int32_t height = decoder.get(CV_CAP_PROP_FRAME_HEIGHT);
  int32_t width  = decoder.get(CV_CAP_PROP_FRAME_WIDTH);

  cout << "Reading sequence " << args_h.get_input() << "..." << endl;

  cout << "        height: " << height     << endl;
  cout << "         width: " << width      << endl;

  typedef vector<Mat>                                                FramesVec;
  vector<Mat> frames;
  frames.reserve(decoder.get(CV_CAP_PROP_FRAME_COUNT));

  Mat frame;

  while (decoder.read(frame))
    frames.push_back(frame.clone());

  decoder.release();
  cout << frames.size() << " frames read." << endl << endl;

  /****************************************************************************
   * Initialization of graphical components and video streams.                *
   ****************************************************************************/

  unique_ptr<Mat> arrows_image;
  unique_ptr<Mat> optical_flow_hsv;

  unique_ptr<GridWindow> window;
  unique_ptr<VideoWriter> record_stream;

  if (args_h.get_visualization() || args_h.get_record()) {
    arrows_image     = unique_ptr<Mat>(new Mat(height, width, CV_8UC3));
    optical_flow_hsv = unique_ptr<Mat>(new Mat(height, width, CV_8UC3));

    if (!args_h.get_split_vis()) {
      TextProperties::TextPropertiesPtr title_properties = nullptr;

      if (args_h.get_record()) {
        title_properties = make_shared<TextProperties>(
          TextProperties::Font::FONT_DUPLEX,
          0.8
        );
      }
      else
        title_properties = make_shared<TextProperties>();

      window = unique_ptr<GridWindow>(
        new GridWindow(
          "LaBGen-OF",
          (args_h.get_v_height() > 0) ? args_h.get_v_height() : height,
          (args_h.get_v_width() > 0) ? args_h.get_v_width() : width,
          2,
          3,
          title_properties
        )
      );

      if (args_h.get_keep_ratio())
        window->keep_ratio();

      if (args_h.get_record()) {
        const Mat& buffer = window->get_buffer();

        record_stream = unique_ptr<VideoWriter>(
          new VideoWriter(
            args_h.get_record_path(),
            CV_FOURCC('M','J','P','G'),
            args_h.get_record_fps(),
            Size(buffer.cols, buffer.rows)
          )
        );
      }
    }
  }

  /****************************************************************************
   * Processing.                                                              *
   ****************************************************************************/

  cout << "Start processing..." << endl;

  /* Initialization of the background matrix. */
  Mat background = Mat(height, width, CV_8UC3);

  /* Initialization of the LaBGen-OF algorithm. */
  LaBGen_OF labgen_of(
    height,
    width,
    args_h.get_a_param(),
    args_h.get_s_param(),
    args_h.get_n_param(),
    args_h.get_p_param(),
    args_h.get_t_param(),
    frames.front()
  );

  /* Processing loop. */
  cout << endl << "Processing..." << endl;
  bool first_frame = true;

  FramesVec::const_iterator begin   = frames.begin();
  FramesVec::const_iterator it      = begin + 1;
  FramesVec::const_iterator prev_it = begin;
  FramesVec::const_iterator end     = frames.end();

  for (
    int32_t pass = 0, passes = (args_h.get_p_param() + 1) / 2;
    pass < passes;
    ++pass
  ) {
    cout << endl << "Processing pass number ";
    cout << lexical_cast<string>((pass * 2) + 1) << "..." << endl;

    bool forward = true;

    do {
      labgen_of.insert(*it);

      /* Visualization. */
      if (args_h.get_visualization() || args_h.get_record()) {
        OpticalFlow::get_arrows_image(
          *prev_it,
          labgen_of.get_vector_field(),
          *arrows_image
        );

        OpticalFlow::get_color_image(
          labgen_of.get_vector_field(),
          *optical_flow_hsv
        );

        labgen_of.generate_background(background);

        if (args_h.get_split_vis()) {
          imshow("Input video", *it);
          imshow("LaBGen-OF", background);
          imshow("Vector field", *arrows_image);
          imshow("Optical flow (HSV)", *optical_flow_hsv);
          imshow("Segmentation map", labgen_of.get_segmentation_map());
        }
        else {
          window->display(*it, 0);
          window->put_title("Input video", 0);

          window->display(background, 2);
          window->put_title("LaBGen-OF", 2);

          window->display(*arrows_image, 3);
          window->put_title("Vector field", 3);

          window->display(*optical_flow_hsv, 4);
          window->put_title("Optical flow (HSV)", 4);

          window->display(labgen_of.get_segmentation_map(), 5);
          window->put_title("Segmentation map", 5);

          if (args_h.get_visualization())
            window->refresh();

          if (args_h.get_record())
            *record_stream << window->get_buffer();
        }

        waitKey(args_h.get_wait());
      }

      /* Move iterator. */
      prev_it = it;
      it = (forward) ? ++it : --it;

      /* If iterator is at the end. */
      if (it == end) {
        /* Kitchen with the iterator. */
        if (pass == (passes - 1))
          break;

        it -= 2;
        forward = false;

        cout << endl << "Processing pass number ";
        cout << lexical_cast<string>((pass + 1) * 2) << "..." << endl;
      }
    } while (it != begin);
  }

  /* Compute background and write it. */
  stringstream output_file;

  output_file << args_h.get_output() << "/output_"
              << args_h.get_a_param() << "_"
              << args_h.get_s_param() << "_"
              << args_h.get_n_param() << "_"
              << args_h.get_p_param() << "_"
              << args_h.get_t_param_int() << ".png";

  labgen_of.generate_background(background);

  cout << "Writing " << output_file.str() << "..." << endl;
  imwrite(output_file.str(), background);

  /* Cleaning. */
  if (args_h.get_visualization()) {
    cout << endl << "Press any key to quit..." << endl;
    waitKey(0);
    destroyAllWindows();

    if (args_h.get_record())
      record_stream->release();
  }

  /* Bye. */
  return EXIT_SUCCESS;
}

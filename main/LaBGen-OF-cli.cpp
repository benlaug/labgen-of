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
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <labgen-of/LaBGen_OF.hpp>
#include <labgen-of/GridWindow.hpp>

using namespace cv;
using namespace std;
using namespace boost::program_options;
using namespace ns_labgen_of;

/******************************************************************************
 * Main program                                                               *
 ******************************************************************************/

int main(int argc, char** argv) {
  /****************************************************************************
   * Argument(s) handling.                                                    *
   ****************************************************************************/

  options_description opt_desc(
    "LaBGen - Copyright - Benjamin Laugraud <blaugraud@ulg.ac.be> - 2017\n"
    "http://www.montefiore.ulg.ac.be/~blaugraud\n"
    "http://www.telecom.ulg.ac.be/labgen\n\n"
    "Usage: ./LaBGen-OF-cli [options]"
  );

  opt_desc.add_options()
    (
      "help,h",
      "print this help message"
    )
    (
      "input,i",
      value<string>(),
      "path to the input sequence"
    )
    (
      "output,o",
      value<string>(),
      "path to the output folder"
    )
    (
      "a-parameter,a",
      value<string>(),
      "name of the optical flow algorithm to use (A parameter)"
    )
    (
      "s-parameter,s",
      value<int32_t>()->multitoken(),
      "value of the S parameter"
    )
    (
      "n-parameter,n",
      value<int32_t>(),
      "value of the N parameter"
    )
    (
      "p-parameter,p",
      value<int32_t>(),
      "value of the P parameter"
    )
    (
      "t-parameter,t",
      value<int32_t>(),
      "value of the T parameter (the given value will be divided by 100)"
    )
    (
      "default,d",
      "use the default set of parameters"
    )
    (
      "visualization,v",
      "enable visualization"
    )
    (
      "split-vis,l",
      "split the visualization items in separated windows"
    )
    (
      "wait,w",
      value<int32_t>()->default_value(1),
      "time to wait (in ms) between the processing of two frames with "
      "visualization"
    )
  ;

  variables_map vars_map;
  store(parse_command_line(argc, argv, opt_desc), vars_map);
  notify(vars_map);

  /* Help message. */
  if (vars_map.count("help")) {
    cout << opt_desc << endl;
    return EXIT_SUCCESS;
  }

  /*
   * Welcome message.
   */

  cout << "===========================================================" << endl;
  cout << "= LaBGen-OF                                                =" << endl;
  cout << "===========================================================" << endl;
  cout << "= Copyright - Benjamin Laugraud - 2017                    =" << endl;
  cout << "= http://www.montefiore.ulg.ac.be/~blaugraud              =" << endl;
  cout << "= http://www.telecom.ulg.ac.be/labgen                     =" << endl;
  cout << "===========================================================" << endl;
  cout << endl;

  /*
   * Extract parameters and sanity check.
   */

  string  a_param     = "";
  int32_t s_param     = 0;
  int32_t n_param     = 0;
  int32_t p_param     = 0;
  int32_t t_param_int = 0;
  double  t_param     = 0;

  /* "input" */
  if (!vars_map.count("input"))
    throw runtime_error("You must provide the path of the input sequence!");

  string sequence(vars_map["input"].as<string>());

  /* "output" */
  if (!vars_map.count("output"))
    throw runtime_error("You must provide the path of the output folder!");

  string output(vars_map["output"].as<string>());

  /* "default" */
  bool default_set = vars_map.count("default");

  if (default_set) {
    a_param     = "deep_flow";
    s_param     = 119;
    n_param     = 8;
    p_param     = 3;
    t_param_int = 4;
    t_param     = 0.04;
  }

  /* Other parameters. */
  if (!default_set) {
    if (!default_set && !vars_map.count("a-parameter")) {
      throw runtime_error(
        "You must provide the name of the optical flow algorithm (A parameter) "
        "to use!"
      );
    }

    /* "a-parameter" */
    a_param = vars_map["a-parameter"].as<string>();

    /* "s-parameter" */
    if (!vars_map.count("s-parameter"))
      throw runtime_error("You must provide the S parameter!");

    s_param = vars_map["s-parameter"].as<int32_t>();

    if (s_param < 1)
      throw runtime_error("The S parameter must be positive!");

    /* "n-parameter" */
    if (!vars_map.count("n-parameter"))
      throw runtime_error("You must provide the N parameter!");

    n_param = vars_map["n-parameter"].as<int32_t>();

    if (n_param < 0) {
      throw runtime_error(
        "The N parameter must be positive (0 = pixel-level)!"
      );
    }

    /* "p-parameter" */
    if (!vars_map.count("p-parameter"))
      throw runtime_error("You must provide the P parameter!");

    p_param = vars_map["p-parameter"].as<int32_t>();

    if (p_param < 1)
      throw runtime_error("The P parameter must be positive!");

    if (p_param % 2 != 1)
      throw runtime_error("The P parameter must be odd!");

    /* "t-parameter" */
    if (!vars_map.count("t-parameter"))
      throw runtime_error("You must provide the T parameter!");

    t_param_int = vars_map["t-parameter"].as<int32_t>();

    if (t_param_int <= 0)
      throw runtime_error("The T parameter must be positive!");

    if (t_param_int >= 100)
      throw runtime_error("The T parameter must be smaller than 100 (1)!");

    t_param = t_param_int / 100.;
  }

  /* "visualization" */
  bool visualization = vars_map.count("visualization");

  /* "split-vis" */
  bool split_vis = vars_map.count("split-vis");

  if (split_vis && !visualization) {
    cerr << "/!\\ The split-vis option without visualization will be ignored!";
    cerr << endl << endl;
  }

  /* "wait" */
  int32_t wait = vars_map["wait"].as<int32_t>();

  if ((wait != 1) && !visualization) {
    cerr << "/!\\ The wait option without visualization will be ignored!";
    cerr << endl << endl;
  }

  if ((wait < 0) && visualization) {
    throw runtime_error(
      "The wait parameter must be positive!"
    );
  }

  /* Display parameters to the user. */
  cout << "Input sequence: "      << sequence      << endl;
  cout << "   Output path: "      << output        << endl;
  cout << "             A: "      << a_param       << endl;
  cout << "             S: "      << s_param       << endl;
  if (n_param > 0)
  cout << "             N: "      << n_param       << endl;
  else
  cout << "             N: pixel" << endl;
  cout << "             P: "      << p_param       << endl;
  cout << "             T: "      << t_param       << endl;
  cout << " Visualization: "      << visualization << endl;
  if (visualization)
  cout << "     Split vis: "      << split_vis     << endl;
  if (visualization)
  cout << "     Wait (ms): "      << wait          << endl;
  cout << endl;

  /****************************************************************************
   * Reading sequence.                                                        *
   ****************************************************************************/

  VideoCapture decoder(sequence);

  if (!decoder.isOpened())
    throw runtime_error("Cannot open the '" + sequence + "' sequence.");

  int32_t height = decoder.get(CV_CAP_PROP_FRAME_HEIGHT);
  int32_t width  = decoder.get(CV_CAP_PROP_FRAME_WIDTH);

  cout << "Reading sequence " << sequence << "..." << endl;

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
   * Processing.                                                              *
   ****************************************************************************/

  cout << "Start processing..." << endl;

  /* Initialization of the background matrix. */
  Mat background = Mat(height, width, CV_8UC3);

  /* Initialization of the LaBGen-OF algorithm. */
  LaBGen_OF labgen_of(
    height,
    width,
    a_param,
    s_param,
    n_param,
    p_param,
    t_param,
    frames.front()
  );

  /* Processing loop. */
  cout << endl << "Processing..." << endl;
  bool first_frame = true;

  FramesVec::const_iterator begin = frames.begin();
  FramesVec::const_iterator it    = begin + 1;
  FramesVec::const_iterator end   = frames.end();

  unique_ptr<Mat> arrows_image;
  unique_ptr<Mat> optical_flow_hsv;
  unique_ptr<GridWindow> window;

  if (visualization) {
    arrows_image     = unique_ptr<Mat>(new Mat(height, width, CV_8UC3));
    optical_flow_hsv = unique_ptr<Mat>(new Mat(height, width, CV_8UC3));

    if (!split_vis)
      window = unique_ptr<GridWindow>(new GridWindow("LaBGen-OF", height, width, 2, 3));
  }

  for (int32_t pass = 0, passes = (p_param + 1) / 2; pass < passes; ++pass) {
    cout << endl << "Processing pass number ";
    cout << boost::lexical_cast<string>((pass * 2) + 1) << "..." << endl;

    bool forward = true;

    do {
      labgen_of.insert(*it);

      /* Visualization. */
      if (visualization) {
        OpticalFlow::get_arrows_image(*it, labgen_of.get_vector_field(), *arrows_image);
        OpticalFlow::get_color_image(labgen_of.get_vector_field(), *optical_flow_hsv);
        labgen_of.generate_background(background);

        if (split_vis) {
          imshow("Input video", *it);
          imshow("Estimated background", background);
          imshow("Vector field", *arrows_image);
          imshow("Optical flow (HSV)", *optical_flow_hsv);
          imshow("Segmentation map", labgen_of.get_segmentation_map());
        }
        else {
          window->display(*it, 0);
          window->display(background, 2);
          window->display(*arrows_image, 3);
          window->display(*optical_flow_hsv, 4);
          window->display(labgen_of.get_segmentation_map(), 5);
        }

        waitKey(wait);
      }

      /* Move iterator. */
      it = (forward) ? ++it : --it;

      /* If iterator is at the end. */
      if (it == end) {
        /* Kitchen with the iterator. */
        if (pass == (passes - 1))
          break;

        it -= 2;
        forward = false;

        cout << endl << "Processing pass number ";
        cout << boost::lexical_cast<string>((pass + 1) * 2) << "..." << endl;
      }
    } while (it != begin);
  }

  /* Compute background and write it. */
  stringstream output_file;
  output_file << output << "/output_" << a_param << "_" << s_param << "_" <<
                 n_param << "_" << p_param << "_" << t_param_int << ".png";

  labgen_of.generate_background(background);

  cout << "Writing " << output_file.str() << "..." << endl;
  imwrite(output_file.str(), background);

  /* Cleaning. */
  if (visualization) {
    cout << endl << "Press any key to quit..." << endl;
    waitKey(0);
    destroyAllWindows();
  }

  /* Bye. */
  return EXIT_SUCCESS;
}

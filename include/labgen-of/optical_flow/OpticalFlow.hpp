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
#pragma once

#include <cstddef>
#include <string>

#include <opencv2/core/core.hpp>

namespace ns_labgen_of {
  /* ======================================================================== *
   * OpticalFlow                                                              *
   * ======================================================================== */

  class OpticalFlow {
    protected:

      OpticalFlow() = default;

      virtual ~OpticalFlow() = default;

    public:

      virtual void compute_flow(
        const cv::Mat& previous_frame,
        const cv::Mat& current_frame,
        cv::Mat& optical_flow
      ) = 0;

      virtual int get_frame_type() = 0;

      static void read_opt_file(
        const std::string& opt_path,
        cv::Mat& optical_flow
      );

      static void write_opt_file(
        const std::string& opt_path,
        const cv::Mat& optical_flow
      );

      static void get_l2_norms(
        const cv::Mat& optical_flow,
        cv::Mat& l2_norms
      );

      static void get_angles(
        const cv::Mat& optical_flow,
        cv::Mat& angles,
        bool degrees = true
      );

      static void get_color_image(
        const cv::Mat& optical_flow,
        cv::Mat& color_image,
        bool inverse_s_and_v = false
      );

      static void get_arrows_image(
        const cv::Mat& input_frame,
        const cv::Mat& optical_flow,
        cv::Mat& arrows_image,
        size_t step_px = 5,
        double visibility_factor = 5
      );

    protected:

      static void check_optical_flow(const cv::Mat& optical_flow);
  };
} /* ns_labgen_of */

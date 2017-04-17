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
#include <vector>

#include <opencv2/core/core.hpp>

namespace ns_labgen_of {
  /* ======================================================================== *
   * Utils                                                                    *
   * ======================================================================== */

  class Utils {
    public:

      typedef std::vector<cv::Rect>                                     ROIs;

    public:

      static std::string getMethod(std::string method);

      static ROIs getROIs(size_t height, size_t width, size_t segments);

      static ROIs getROIs(size_t height, size_t width);

      static void normalize_mat(cv::Mat& input, double max = 1);

      static void normalize_mat(
        const cv::Mat& input,
        cv::Mat& output,
        double max = 1
      );
  };
} /* ns_labgen_of */

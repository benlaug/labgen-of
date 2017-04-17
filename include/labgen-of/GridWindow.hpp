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
#include <cstdint>
#include <unordered_set>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Utils.hpp"

namespace ns_labgen_of {
  /* ======================================================================== *
   * GridWindow                                                               *
   * ======================================================================== */

  class GridWindow {
    protected:

      typedef Utils::ROIs                                                 ROIs;

    protected:

      ROIs rois;
      std::string window_name;
      int32_t height;
      int32_t width;
      int32_t rows;
      int32_t cols;
      cv::Mat buffer;
      static std::unordered_set<std::string> available_windows;

    public:

      GridWindow(
        std::string window_name,
        int32_t height,
        int32_t width,
        int32_t rows = 1,
        int32_t cols = 1
      );

      virtual ~GridWindow();

      void display(const cv::Mat& mat, int32_t index = 0);

      void display(const cv::Mat& mat, int32_t row, int32_t col);
  };
} /* ns_labgen_of */

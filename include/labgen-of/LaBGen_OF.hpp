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
#include <memory>
#include <string>

#include <opencv2/core/core.hpp>

#include "optical_flow/OpticalFlow.hpp"
#include "History.hpp"

namespace ns_labgen_of {
  /* ======================================================================== *
   * LaBGen_OF                                                                *
   * ======================================================================== */

  class LaBGen_OF {
    protected:

      size_t height;
      size_t width;
      std::string a;
      int32_t s;
      int32_t n;
      int32_t p;
      double t;
      cv::Mat previous_frame;
      cv::Mat _previous_frame;
      std::shared_ptr<OpticalFlow> optical_flow;
      cv::Mat vector_field;
      cv::Mat vector_field_l2;
      cv::Mat segmentation_map;
      ns_internals::PatchesHistory history;

    public:

      LaBGen_OF(
        size_t height,
        size_t width,
        std::string a,
        int32_t s,
        int32_t n,
        int32_t p,
        double t,
        const cv::Mat& first_frame
      );

      void insert(const cv::Mat& current_frame);

      void generate_background(cv::Mat& background) const;

      size_t get_height() const;

      size_t get_width() const;

      std::string get_a() const;

      int32_t get_s() const;

      int32_t get_n() const;

      int32_t get_p() const;

      double get_t() const;

      const cv::Mat& get_vector_field() const;

      const cv::Mat& get_normalized_magnitudes() const;

      const cv::Mat& get_segmentation_map() const;

    protected:

      static void threshold_magnitude(
        const cv::Mat& l2_norms,
        cv::Mat& result,
        double threshold
      );
  };
} /* ns_labgen_of */

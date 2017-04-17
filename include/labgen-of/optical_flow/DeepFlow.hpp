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

#include <opencv2/optflow.hpp>

#include "OpticalFlow.hpp"

namespace ns_labgen_of {
  /* ======================================================================== *
   * DeepFlow                                                                 *
   * ======================================================================== */

  class DeepFlow : public OpticalFlow {
    protected:

      cv::Ptr<cv::DenseOpticalFlow> deep_flow;

    public:

      DeepFlow();

      virtual ~DeepFlow() = default;

    public:

      virtual void compute_flow(
        const cv::Mat& previous_frame,
        const cv::Mat& current_frame,
        cv::Mat& optical_flow
      );

      virtual int get_frame_type();
  };
} /* ns_labgen_of */

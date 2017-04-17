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
#include <stdexcept>

#include <labgen-of/optical_flow/DeepFlow.hpp>
#include <labgen-of/optical_flow/DISFlow.hpp>
#include <labgen-of/optical_flow/DualTVL1Flow.hpp>
#include <labgen-of/optical_flow/FarnebackFlow.hpp>
#include <labgen-of/optical_flow/OpticalFlowFactory.hpp>
#include <labgen-of/optical_flow/SimpleFlow.hpp>
#include <labgen-of/optical_flow/SparseToDenseFlow.hpp>

using namespace std;
using namespace ns_labgen_of;

/* ========================================================================== *
 * OpticalFlowFactory                                                         *
 * ========================================================================== */

shared_ptr<OpticalFlow> OpticalFlowFactory::get_optical_flow(string algorithm) {
  if (algorithm == "deep_flow")
    return shared_ptr<OpticalFlow>(new DeepFlow);
  else if (algorithm == "sparse_to_dense")
    return shared_ptr<OpticalFlow>(new SparseToDenseFlow);
  else if (algorithm == "dis_flow")
    return shared_ptr<OpticalFlow>(new DISFlow);
  else if (algorithm == "farneback")
    return shared_ptr<OpticalFlow>(new FarnebackFlow);
  else if (algorithm == "simple_flow")
    return shared_ptr<OpticalFlow>(new SimpleFlow);
  else if (algorithm == "dual_tvl1")
    return shared_ptr<OpticalFlow>(new DualTVL1Flow);
  else {
    throw runtime_error(
      "The optical flow algorithm " + algorithm + " is not supported."
    );
  }
}

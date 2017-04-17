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
#ifndef _NS_LABGEN_OF_NS_INTERNALS_HISTORY_IPP_
#error "History.hpp must be included instead of History.ipp"
#else
/******************************************************************************
 * Operator(s) overloading                                                    *
 ******************************************************************************/

inline bool operator<(const HistoryMat& lhs, const HistoryMat& rhs) {
  return lhs.positives < rhs.positives;
}

/******************************************************************************/

inline bool operator<=(const HistoryMat& lhs, const HistoryMat& rhs) {
  return lhs.positives <= rhs.positives;
}

/******************************************************************************/

inline bool operator==(const HistoryMat& lhs, const HistoryMat& rhs) {
  return lhs.positives == rhs.positives;
}

/******************************************************************************/

inline bool operator<(const HistoryMat& lhs, const uint32_t& rhs) {
  return lhs.positives < rhs;
}

/******************************************************************************/

inline bool operator<=(const HistoryMat& lhs, const uint32_t& rhs) {
  return lhs.positives <= rhs;
}

/******************************************************************************/

inline bool operator==(const HistoryMat& lhs, const uint32_t& rhs) {
  return lhs.positives == rhs;
}

/******************************************************************************/

inline bool operator<(const uint32_t& lhs, const HistoryMat& rhs) {
  return lhs < rhs.positives;
}

/******************************************************************************/

inline bool operator<=(const uint32_t& lhs, const HistoryMat& rhs) {
  return lhs <= rhs.positives;
}

/******************************************************************************/

inline bool operator==(const uint32_t& lhs, const HistoryMat& rhs) {
  return lhs == rhs.positives;
}
#endif /* _NS_LABGEN_OF_NS_INTERNALS_HISTORY_IPP_ */

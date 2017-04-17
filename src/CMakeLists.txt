# Copyright - Benjamin Laugraud <blaugraud@ulg.ac.be> - 2017
# http://www.montefiore.ulg.ac.be/~blaugraud
# http://www.telecom.ulg.ac.be/labgen
#
# This file is part of LaBGen-OF.
#
# LaBGen-OF is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LaBGen-OF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LaBGen-OF.  If not, see <http://www.gnu.org/licenses/>.
file(
  GLOB_RECURSE
  LaBGen-OF_src
  *.cpp
)

# Shared library.
add_library(
  LaBGen-OF_shared
  SHARED
  ${LaBGen-OF_src}
  ${LaBGen-OF_include}
)

set_target_properties(
  LaBGen-OF_shared
  PROPERTIES
  OUTPUT_NAME
  LaBGen-OF
)

target_link_libraries(
  LaBGen-OF_shared
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
)

# Static library.
add_library(
  LaBGen-OF_static
  STATIC
  ${LaBGen-OF_src}
  ${LaBGen-OF_include}
)

set_target_properties(
  LaBGen-OF_static
  PROPERTIES
  OUTPUT_NAME
  LaBGen-OF
)

target_link_libraries(
  LaBGen-OF_static
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
)

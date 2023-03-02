# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(GenerateExportHeader)

add_library(
  testSharedLibrary SHARED
  src/TestSharedLibrary.cpp
)
generate_export_header(testSharedLibrary)
target_include_directories(testSharedLibrary
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(testSharedLibrary
  PRIVATE
    Qt5::Core
)

add_executable(
  testExecutableDynamic
  src/TestExecutableDynamic.cpp
)
target_link_libraries(testExecutableDynamic
  PRIVATE
    testSharedLibrary
    Qt5::Core
)
# On MSVC, also check that delay load libraries works
if(MSVC)
  target_link_options(testExecutableDynamic
    PRIVATE "/DELAYLOAD:$<TARGET_FILE:testSharedLibrary>"
  )
endif()

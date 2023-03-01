# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(Mdt_Catch2Qt STATIC Catch2QString.cpp)
target_include_directories(Mdt_Catch2Qt
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(Mdt_Catch2Qt PUBLIC Qt5::Core)
add_library(Mdt::Catch2Qt ALIAS Mdt_Catch2Qt)

add_library(Mdt_Catch2Main STATIC Catch2Main.cpp)
target_link_libraries(Mdt_Catch2Main PUBLIC Catch2::Catch2)
add_library(Mdt::Catch2Main ALIAS Mdt_Catch2Main)


if(BUILD_BENCHMARKS)
  target_compile_definitions(Mdt_Catch2Main
    PUBLIC
      CATCH_CONFIG_ENABLE_BENCHMARKING
  )
endif()

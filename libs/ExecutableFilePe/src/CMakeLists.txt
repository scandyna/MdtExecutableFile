# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(Mdt_ExecutableFilePe STATIC
  
)

add_library(Mdt::ExecutableFilePe ALIAS Mdt_ExecutableFilePe)

target_compile_features(Mdt_ExecutableFilePe PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFilePe
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(Mdt_ExecutableFilePe
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFilePe
  PUBLIC
    Qt5::Core
    Mdt::ExecutableFile_Common
#   PRIVATE
#     Boost::boost
)

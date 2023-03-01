# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

# include(GenerateExportHeader)
include(MdtInstallLibrary)

add_library(Mdt_ExecutableFile_Common STATIC
  Mdt/ExecutableFile/ByteArraySpan.cpp
)

add_library(Mdt::ExecutableFile_Common ALIAS Mdt_ExecutableFile_Common)

target_compile_features(Mdt_ExecutableFile_Common PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFile_Common
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(Mdt_ExecutableFile_Common
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFile_Common
  PUBLIC
    Qt5::Core
#   PRIVATE
#     Boost::boost
)

# generate_export_header(Mdt_ExecutableFile_Common)

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(Mdt_ExecutableFileElf STATIC
  Mdt/ExecutableFile/Elf/Algorithm.cpp
)

add_library(Mdt::ExecutableFileElf ALIAS Mdt_ExecutableFileElf)

target_compile_features(Mdt_ExecutableFileElf PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFileElf
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(Mdt_ExecutableFileElf
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFileElf
  PUBLIC
    Qt5::Core
    Mdt::ExecutableFile_Common
#   PRIVATE
#     Boost::boost
)

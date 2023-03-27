# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

if(BUILD_TESTS OR BUILD_BENCHMARKS)
  add_subdirectory(Catch2)
  add_subdirectory(TestLib)
  add_subdirectory(TestBinaries)
endif()

add_subdirectory(ExecutableFile_Common)
add_subdirectory(ExecutableFileElf)
add_subdirectory(ExecutableFilePe)
add_subdirectory(ExecutableFile_Core)

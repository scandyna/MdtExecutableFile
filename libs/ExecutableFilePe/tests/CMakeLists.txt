# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)


mdt_add_test(
  NAME PeFileReaderTest
  TARGET peFileReaderTest
  DEPENDENCIES Mdt::ExecutableFilePe Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileReaderTest.cpp
)

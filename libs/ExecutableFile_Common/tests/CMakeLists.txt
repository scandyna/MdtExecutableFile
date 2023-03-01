# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)

mdt_add_test(
  NAME ByteArraySpanTest
  TARGET byteArraySpanTest
  DEPENDENCIES Mdt::ExecutableFile_Common Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ByteArraySpanTest.cpp
)

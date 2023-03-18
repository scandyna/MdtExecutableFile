# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)

mdt_add_test(
  NAME AlgorithmTest
  TARGET algorithmTest
  DEPENDENCIES Mdt::ExecutableFile_Common Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AlgorithmTest.cpp
)

mdt_add_test(
  NAME PlatformTest
  TARGET platformTest
  DEPENDENCIES Mdt::ExecutableFile_Common Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PlatformTest.cpp
)

mdt_add_test(
  NAME ByteArraySpanTest
  TARGET byteArraySpanTest
  DEPENDENCIES Mdt::ExecutableFile_Common Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ByteArraySpanTest.cpp
)

mdt_add_test(
  NAME FileMapperTest
  TARGET fileMapperTest
  DEPENDENCIES Mdt::ExecutableFile_Common TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/FileMapperTest.cpp
)

mdt_add_test(
  NAME ExecutableFileReaderUtilsTest
  TARGET executableFileReaderUtilsTest
  DEPENDENCIES Mdt::ExecutableFile_Common TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileReaderUtilsTest.cpp
)

mdt_add_test(
  NAME ExecutableFileReaderUtilsErrorTest
  TARGET executableFileReaderUtilsErrorTest
  DEPENDENCIES Mdt::ExecutableFile_Common Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileReaderUtilsErrorTest.cpp
)

mdt_add_test(
  NAME RPathTest
  TARGET rPathTest
  DEPENDENCIES Mdt::ExecutableFile_Common TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RPathTest.cpp
)

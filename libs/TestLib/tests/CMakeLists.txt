# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)

mdt_add_test(
  NAME TestFileUtilsTest
  TARGET testFileUtilsTest
  DEPENDENCIES TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/TestFileUtilsTest.cpp
)

mdt_add_test(
  NAME RPathTestUtilsTest
  TARGET rPathTestUtilsTest
  DEPENDENCIES TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RPathTestUtilsTest.cpp
)

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)

mdt_add_test(
  NAME ElfAlgorithmTest
  TARGET elfAlgorithmTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfAlgorithmTest.cpp
)

mdt_add_test(
  NAME ElfStringTableTest
  TARGET elfStringTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfStringTableTest.cpp
)

mdt_add_test(
  NAME ElfStringTableErrorTest
  TARGET elfStringTableErrorTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfStringTableErrorTest.cpp
)

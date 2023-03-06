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
  NAME ElfProgramHeaderTest
  TARGET elfProgramHeaderTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramHeaderTest.cpp
)

mdt_add_test(
  NAME ElfProgramHeaderTableTest
  TARGET elfProgramHeaderTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramHeaderTableTest.cpp
)

mdt_add_test(
  NAME ElfStringTableTest
  TARGET elfStringTableTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfStringTableTest.cpp
)

mdt_add_test(
  NAME ElfStringTableErrorTest
  TARGET elfStringTableErrorTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfStringTableErrorTest.cpp
)

mdt_add_test(
  NAME ElfDynamicSectionTest
  TARGET elfDynamicSectionTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfDynamicSectionTest.cpp
)

mdt_add_test(
  NAME ElfDynamicSectionErrorTest
  TARGET elfDynamicSectionErrorTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfDynamicSectionErrorTest.cpp
)

mdt_add_test(
  NAME ElfNoteSectionTest
  TARGET elfNoteSectionTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfNoteSectionTest.cpp
)

mdt_add_test(
  NAME ElfNoteSectionTableTest
  TARGET elfNoteSectionTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfNoteSectionTableTest.cpp
)

# mdt_add_test(
#   NAME ElfNoteSectionReaderWriterTest
#   TARGET elfNoteSectionReaderWriterTest
#   DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
#   SOURCE_FILES
#     src/ElfNoteSectionReaderWriterTest.cpp
# )

# mdt_add_test(
#   NAME ElfNoteSectionReaderWriterErrorTest
#   TARGET elfNoteSectionReaderWriterErrorTest
#   DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
#   SOURCE_FILES
#     src/ElfNoteSectionReaderWriterErrorTest.cpp
# )

mdt_add_test(
  NAME ElfProgramInterpreterSectionTest
  TARGET elfProgramInterpreterSectionTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramInterpreterSectionTest.cpp
)

mdt_add_test(
  NAME ElfHashTableTest
  TARGET elfHashTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfHashTableTest.cpp
)

mdt_add_test(
  NAME ElfGnuHashTableTest
  TARGET elfGnuHashTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGnuHashTableTest.cpp
)

mdt_add_test(
  NAME ElfGlobalOffsetTableTest
  TARGET elfGlobalOffsetTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGlobalOffsetTableTest.cpp
)

mdt_add_test(
  NAME ElfSymbolTableTest
  TARGET elfSymbolTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSymbolTableTest.cpp
)

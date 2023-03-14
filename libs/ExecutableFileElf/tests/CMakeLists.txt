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
  NAME ElfFileHeaderWriterTest
  TARGET elfFileHeaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileHeaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfSectionHeaderTest
  TARGET elfSectionHeaderTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSectionHeaderTest.cpp
)

mdt_add_test(
  NAME ElfSectionHeaderTableTest
  TARGET elfSectionHeaderTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSectionHeaderTableTest.cpp
)

mdt_add_test(
  NAME ElfSectionHeaderWriterTest
  TARGET elfSectionHeaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSectionHeaderWriterTest.cpp
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
  NAME ElfProgramHeaderReaderTest
  TARGET elfProgramHeaderReaderTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramHeaderReaderTest.cpp
)

mdt_add_test(
  NAME ElfProgramHeaderReaderErrorTest
  TARGET elfProgramHeaderReaderErrorTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramHeaderReaderErrorTest.cpp
)

mdt_add_test(
  NAME ElfProgramHeaderWriterTest
  TARGET elfProgramHeaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramHeaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfOffsetRangeTest
  TARGET elfOffsetRangeTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfOffsetRangeTest.cpp
)

mdt_add_test(
  NAME ElfSectionSegmentUtilsTest
  TARGET elfSectionSegmentUtilsTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSectionSegmentUtilsTest.cpp
)

mdt_add_test(
  NAME ElfFileAllHeadersTestUtilsTest
  TARGET elfFileAllHeadersTestUtilsTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileAllHeadersTestUtilsTest.cpp
)

mdt_add_test(
  NAME ElfFileAllHeadersTest
  TARGET elfFileAllHeadersTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileAllHeadersTest.cpp
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
  NAME ElfStringTableWriterTest
  TARGET elfStringTableWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfStringTableWriterTest.cpp
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
  NAME ElfDynamicSectionWriterTest
  TARGET elfDynamicSectionWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfDynamicSectionWriterTest.cpp
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

mdt_add_test(
  NAME ElfNoteSectionReaderWriterTest
  TARGET elfNoteSectionReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfNoteSectionReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfNoteSectionReaderWriterErrorTest
  TARGET elfNoteSectionReaderWriterErrorTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfNoteSectionReaderWriterErrorTest.cpp
)

mdt_add_test(
  NAME ElfProgramInterpreterSectionTest
  TARGET elfProgramInterpreterSectionTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramInterpreterSectionTest.cpp
)

mdt_add_test(
  NAME ElfProgramInterpreterSectionReaderWriterTest
  TARGET elfProgramInterpreterSectionReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfProgramInterpreterSectionReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfHashTableTest
  TARGET elfHashTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfHashTableTest.cpp
)

mdt_add_test(
  NAME ElfHashTableReaderWriterTest
  TARGET elfHashTableReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfHashTableReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfGnuHashTableTest
  TARGET elfGnuHashTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGnuHashTableTest.cpp
)

mdt_add_test(
  NAME ElfGnuHashTableReaderWriterTest
  TARGET elfGnuHashTableReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGnuHashTableReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfGlobalOffsetTableTest
  TARGET elfGlobalOffsetTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGlobalOffsetTableTest.cpp
)

mdt_add_test(
  NAME ElfGlobalOffsetTableReaderWriterTest
  TARGET elfGlobalOffsetTableReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfGlobalOffsetTableReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfSymbolTableTest
  TARGET elfSymbolTableTest
  DEPENDENCIES Mdt::ExecutableFileElf Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSymbolTableTest.cpp
)

mdt_add_test(
  NAME ElfSymbolTableReaderWriterTest
  TARGET elfSymbolTableReaderWriterTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfSymbolTableReaderWriterTest.cpp
)

mdt_add_test(
  NAME ElfFileReaderTest
  TARGET elfFileReaderTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileReaderTest.cpp
)

mdt_add_test(
  NAME ElfFileWriterFileTest
  TARGET elfFileWriterFileTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileWriterFileTest.cpp
)

mdt_add_test(
  NAME ElfFileWriterUnitTest
  TARGET elfFileWriterUnitTest
  DEPENDENCIES Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileWriterUnitTest.cpp
)

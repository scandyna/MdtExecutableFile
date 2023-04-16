# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)

mdt_add_test(
  NAME ELV_NumericTest
  TARGET elv_NumericTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/NumericTest.cpp
)

mdt_add_test(
  NAME ELV_SegmentTypeStringTest
  TARGET elv_SegmentTypeStringTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/SegmentTypeStringTest.cpp
)

mdt_add_test(
  NAME ELV_SectionGraphicsItemDataTest
  TARGET elv_SectionGraphicsItemDataTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/SectionGraphicsItemDataTest.cpp
)

mdt_add_test(
  NAME ELV_SectionGraphicsItemTest
  TARGET elv_SectionGraphicsItemTest
  DEPENDENCIES Mdt::ElfLayoutViewer Qt5::Test
  SOURCE_FILES
    src/SectionGraphicsItemTest.cpp
)

mdt_add_test(
  NAME ELV_SegmentGraphicsItemDataTest
  TARGET elv_SegmentGraphicsItemDataTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/SegmentGraphicsItemDataTest.cpp
)

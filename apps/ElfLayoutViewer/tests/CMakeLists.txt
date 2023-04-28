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

mdt_add_test(
  NAME ELV_SegmentGraphicsItemTest
  TARGET elv_SegmentGraphicsItemTest
  DEPENDENCIES Mdt::ElfLayoutViewer Qt5::Test
  SOURCE_FILES
    src/SegmentGraphicsItemTest.cpp
)

mdt_add_test(
  NAME ELV_HeaderTableGraphicsItemMapIdTest
  TARGET elv_HeaderTableGraphicsItemMapIdTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/HeaderTableGraphicsItemMapIdTest.cpp
)

mdt_add_test(
  NAME ELV_HeaderTableGraphicsItemMapTest
  TARGET elv_HeaderTableGraphicsItemMapTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/HeaderTableGraphicsItemMapTest.cpp
)


mdt_add_test(
  NAME ELV_SectionHeaderTableModelTest
  TARGET elv_SectionHeaderTableModelTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/SectionHeaderTableModelTest.cpp
)

# Some tests that depends on Qt TestLib,
# like QAbstractItemModelTester
# (available in the public API since Qt 5.11)
mdt_add_test(
  NAME ELV_SectionHeaderTableModelQTLTest
  TARGET elv_SectionHeaderTableModelQTLTest
  DEPENDENCIES Mdt::ElfLayoutViewer Qt5::Test
  SOURCE_FILES
    src/SectionHeaderTableModelQTLTest.cpp
)

mdt_add_test(
  NAME ELV_ProgramHeaderTableModelTest
  TARGET elv_ProgramHeaderTableModelTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ProgramHeaderTableModelTest.cpp
)

# Some tests that depends on Qt TestLib,
# like QAbstractItemModelTester
# (available in the public API since Qt 5.11)
mdt_add_test(
  NAME ELV_ProgramHeaderTableModelQTLTest
  TARGET elv_ProgramHeaderTableModelQTLTest
  DEPENDENCIES Mdt::ElfLayoutViewer Qt5::Test
  SOURCE_FILES
    src/ProgramHeaderTableModelQTLTest.cpp
)

mdt_add_test(
  NAME ELV_LayoutViewSegmentLayoutItemTest
  TARGET elv_LayoutViewSegmentLayoutItemTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/LayoutViewSegmentLayoutItemTest.cpp
)

mdt_add_test(
  NAME ELV_LayoutViewSegmentLayoutTest
  TARGET elv_LayoutViewSegmentLayoutTest
  DEPENDENCIES Mdt::ElfLayoutViewer Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/LayoutViewSegmentLayoutTest.cpp
)

mdt_add_test(
  NAME ELV_LayoutViewGraphicsSceneTest
  TARGET elv_LayoutViewGraphicsSceneTest
  DEPENDENCIES Mdt::ElfLayoutViewer Qt5::Test
  SOURCE_FILES
    src/LayoutViewGraphicsSceneTest.cpp
)

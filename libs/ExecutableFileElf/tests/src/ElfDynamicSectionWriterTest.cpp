// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "ElfDynamicSectionTestCommon.h"
#include "ElfFileIoTestUtils.h"
#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include "Mdt/ExecutableFile/Elf/DynamicSectionWriter.h"
#include <QLatin1String>

using Mdt::ExecutableFile::ByteArraySpan;
using Mdt::ExecutableFile::Elf::FileHeader;
using Mdt::ExecutableFile::Elf::Ident;
using Mdt::ExecutableFile::Elf::DynamicSection;

TEST_CASE("dynamicSectionByteCount")
{
  using Mdt::ExecutableFile::Elf::dynamicSectionByteCount;

  Ident ident;
  DynamicSection dynamicSection;

  SECTION("1 entry")
  {
    dynamicSection.addEntry( makeNeededEntry(12) );

    SECTION("32-bit")
    {
      ident = make32BitBigEndianIdent();
      REQUIRE( dynamicSectionByteCount(dynamicSection, ident) == 8 );
    }

    SECTION("64-bit")
    {
      ident = make64BitLittleEndianIdent();
      REQUIRE( dynamicSectionByteCount(dynamicSection, ident) == 16 );
    }
  }

  SECTION("2 entries")
  {
    dynamicSection.addEntry( makeNullEntry() );
    dynamicSection.addEntry( makeNeededEntry(12) );

    SECTION("32-bit")
    {
      ident = make32BitBigEndianIdent();
      REQUIRE( dynamicSectionByteCount(dynamicSection, ident) == 16 );
    }

    SECTION("64-bit")
    {
      ident = make64BitLittleEndianIdent();
      REQUIRE( dynamicSectionByteCount(dynamicSection, ident) == 32 );
    }
  }
}

TEST_CASE("dynamicSectionToArray")
{
  using Mdt::ExecutableFile::Elf::dynamicSectionToArray;

  Ident ident;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  DynamicSection dynamicSection;

  SECTION("1 entry")
  {
    dynamicSection.addEntry( makeRunPathEntry(0x12345678) );

    SECTION("32-bit big-endian")
    {
      uchar expectedArrayData[8] = {
        // d_tag
        0,0,0,0x1D, // 0x1D (29)
        // d_val
        0x12,0x34,0x56,0x78   // 0x12345678
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[8] = {};
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );
      ident = make32BitBigEndianIdent();

      dynamicSectionToArray(array, dynamicSection, ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }

    SECTION("64-bit little-endian")
    {
      uchar expectedArrayData[16] = {
        // d_tag
        0x1D,0,0,0,0,0,0,0, // 0x1D (29)
        // d_val
        0x78,0x56,0x34,0x12,0,0,0,0 // 0x12345678
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[16] = {};
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );
      ident = make64BitLittleEndianIdent();

      dynamicSectionToArray(array, dynamicSection, ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }
  }

  SECTION("2 entries")
  {
    dynamicSection.addEntry( makeNullEntry() );
    dynamicSection.addEntry( makeRunPathEntry(0x12345678) );

    SECTION("32-bit big-endian")
    {
      uchar expectedArrayData[16] = {
        // d_tag
        0,0,0,0,
        // d_val or d_ptr
        0,0,0,0,
        // d_tag
        0,0,0,0x1D, // 0x1D (29)
        // d_val
        0x12,0x34,0x56,0x78   // 0x12345678
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[16] = {};
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );
      ident = make32BitBigEndianIdent();

      dynamicSectionToArray(array, dynamicSection, ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }

    SECTION("64-bit little-endian")
    {
      uchar expectedArrayData[32] = {
        // d_tag
        0,0,0,0,0,0,0,0,
        // d_val or d_ptr
        0,0,0,0,0,0,0,0,
        // d_tag
        0x1D,0,0,0,0,0,0,0, // 0x1D (29)
        // d_val
        0x78,0x56,0x34,0x12,0,0,0,0 // 0x12345678
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[32] = {};
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );
      ident = make64BitLittleEndianIdent();

      dynamicSectionToArray(array, dynamicSection, ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }
  }
}

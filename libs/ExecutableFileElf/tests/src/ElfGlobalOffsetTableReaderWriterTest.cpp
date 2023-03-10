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
#include "ElfFileIoTestUtils.h"
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableReader.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableWriter.h"

using namespace Mdt::ExecutableFile::Elf;
using Mdt::ExecutableFile::ByteArraySpan;


TEST_CASE("globalOffsetTableEntryFromArray")
{
  GlobalOffsetTableEntry entry;
  FileHeader fileHeader;
  ByteArraySpan array;

  SECTION("32-bit big-endian")
  {
    uchar arrayData[4] = {
      0x12,0x34,0x56,0x78 // 0x12345678
    };
    fileHeader = make32BitBigEndianFileHeader();
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    entry = globalOffsetTableEntryFromArray(array, fileHeader.ident);

    REQUIRE( entry.data == 0x12345678 );
  }

  SECTION("64-bit little-endian")
  {
    uchar arrayData[8] = {
      0x56,0x34,0x12,0x90,0x78,0x56,0x34,0x12 // 0x1234567890123456
    };
    fileHeader = make64BitLittleEndianFileHeader();
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    entry = globalOffsetTableEntryFromArray(array, fileHeader.ident);

    REQUIRE( entry.data == 0x1234567890123456 );
  }
}

TEST_CASE("globalOffsetTableEntryToArray")
{
  GlobalOffsetTableEntry entry;
  FileHeader fileHeader;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  SECTION("32-bit big-endian")
  {
    entry.data = 0x12345678;

    uchar expectedArrayData[4] = {
      0x12,0x34,0x56,0x78 // 0x12345678
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    fileHeader = make32BitBigEndianFileHeader();
    uchar arrayData[4];
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setGlobalOffsetTableEntryToArray(array, entry, fileHeader.ident);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }

  SECTION("64-bit little-endian")
  {
    entry.data = 0x1234567890123456;

    uchar expectedArrayData[8] = {
      0x56,0x34,0x12,0x90,0x78,0x56,0x34,0x12 // 0x1234567890123456
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    fileHeader = make64BitLittleEndianFileHeader();
    uchar arrayData[8];
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setGlobalOffsetTableEntryToArray(array, entry, fileHeader.ident);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }
}

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
#include "ElfFileIoTestCommon.h"
#include "ElfFileIoTestUtils.h"
#include "ElfDynamicSectionTestCommon.h"
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/Elf/FileWriter.h"
#include "Mdt/ExecutableFile/Elf/FileOffsetChanges.h"
#include <QLatin1String>

// #include <QDebug>

using namespace Mdt::ExecutableFile;


TEST_CASE("setHalfWord")
{
  using Elf::setHalfWord;
  using Elf::DataFormat;

  uchar charArray[2] = {};
  ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

  SECTION("little-endian")
  {
    setHalfWord(array, 0x1234, DataFormat::Data2LSB);
    REQUIRE( arraysAreEqual(array, {0x34,0x12}) );
  }

  SECTION("big-endian")
  {
    setHalfWord(array, 0x1234, DataFormat::Data2MSB);
    REQUIRE( arraysAreEqual(array, {0x12,0x34}) );
  }
}

TEST_CASE("set32BitWord")
{
  using Elf::set32BitWord;
  using Elf::DataFormat;

  uchar charArray[4] = {};
  ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

  SECTION("little-endian")
  {
    set32BitWord(array, 0x12345678, DataFormat::Data2LSB);
    REQUIRE( arraysAreEqual(array, {0x78,0x56,0x34,0x12}) );
  }

  SECTION("big-endian")
  {
    set32BitWord(array, 0x12345678, DataFormat::Data2MSB);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78}) );
  }
}

TEST_CASE("set64BitWord")
{
  using Elf::set64BitWord;
  using Elf::DataFormat;

  uchar charArray[8] = {0};
  ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

  SECTION("little-endian")
  {
    set64BitWord(array, 0x123456789ABCDEF0, DataFormat::Data2LSB);
    REQUIRE( arraysAreEqual(array, {0xF0,0xDE,0xBC,0x9A,0x78,0x56,0x34,0x12}) );
  }

  SECTION("big-endian")
  {
    set64BitWord(array, 0x123456789ABCDEF0, DataFormat::Data2MSB);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0}) );
  }
}

TEST_CASE("setNWord")
{
  using Elf::setNWord;
  using Elf::DataFormat;
  using Elf::Ident;

  Ident ident;

  SECTION("32-bit little-endian")
  {
    ident = make32BitLittleEndianIdent();
    uchar charArray[4] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setNWord(array, 0x12345678, ident);
    REQUIRE( arraysAreEqual(array, {0x78,0x56,0x34,0x12}) );
  }

  SECTION("32-bit big-endian")
  {
    ident = make32BitBigEndianIdent();
    uchar charArray[4] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setNWord(array, 0x12345678, ident);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78}) );
  }

  SECTION("64-bit little-endian")
  {
    ident = make64BitLittleEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setNWord(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0xF0,0xDE,0xBC,0x9A,0x78,0x56,0x34,0x12}) );
  }

  SECTION("64-bit big-endian")
  {
    ident = make64BitBigEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setNWord(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0}) );
  }
}

TEST_CASE("setAddress")
{
  using Elf::setAddress;
  using Elf::DataFormat;
  using Elf::Ident;

  Ident ident;

  SECTION("64-bit little-endian")
  {
    ident = make64BitLittleEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setAddress(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0xF0,0xDE,0xBC,0x9A,0x78,0x56,0x34,0x12}) );
  }
}

TEST_CASE("setOffset")
{
  using Elf::setOffset;
  using Elf::DataFormat;
  using Elf::Ident;

  Ident ident;

  SECTION("64-bit little-endian")
  {
    ident = make64BitLittleEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setOffset(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0xF0,0xDE,0xBC,0x9A,0x78,0x56,0x34,0x12}) );
  }
}

TEST_CASE("setSignedNWord")
{
  using Elf::setSignedNWord;
  using Elf::DataFormat;
  using Elf::Ident;

  Ident ident;

  SECTION("32-bit little-endian")
  {
    ident = make32BitLittleEndianIdent();
    uchar charArray[4] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setSignedNWord(array, 0x12345678, ident);
    REQUIRE( arraysAreEqual(array, {0x78,0x56,0x34,0x12}) );
  }

  SECTION("32-bit big-endian")
  {
    ident = make32BitBigEndianIdent();
    uchar charArray[4] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setSignedNWord(array, 0x12345678, ident);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78}) );
  }

  SECTION("64-bit little-endian")
  {
    ident = make64BitLittleEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setSignedNWord(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0xF0,0xDE,0xBC,0x9A,0x78,0x56,0x34,0x12}) );
  }

  SECTION("64-bit big-endian")
  {
    ident = make64BitBigEndianIdent();
    uchar charArray[8] = {};
    ByteArraySpan array = arraySpanFromArray( charArray, sizeof(charArray) );

    setSignedNWord(array, 0x123456789ABCDEF0, ident);
    REQUIRE( arraysAreEqual(array, {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0}) );
  }
}

TEST_CASE("replaceBytesInArray")
{
  using Elf::replaceBytesInArray;

  ByteArraySpan array;

  SECTION("A -> B")
  {
    uchar arrayData[1] = {'A'};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    replaceBytesInArray(array, 'B');
    REQUIRE( arraysAreEqual(array, {'B'}) );
  }
}

TEST_CASE("setStringToUnsignedCharArray")
{
  using Elf::setStringToUnsignedCharArray;

  ByteArraySpan array;

  SECTION("empty string")
  {
    uchar arrayData[1] = {0xFF};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setStringToUnsignedCharArray(array, "");
    REQUIRE( array.size == 1 );
    REQUIRE( array.data[0] == '\0' );
    // MSVC does not like below version (C2668)
//     REQUIRE( arraysAreEqual(array, {'\0'}) );
  }

  SECTION("A")
  {
    uchar arrayData[2] = {0xFF,0xFF};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setStringToUnsignedCharArray(array, "A");
    REQUIRE( arraysAreEqual(array, {'A','\0'}) );
  }

  SECTION("ABC")
  {
    uchar arrayData[4] = {0xFF,0xFF,0xFF,0xFF};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setStringToUnsignedCharArray(array, "ABC");
    REQUIRE( arraysAreEqual(array, {'A','B','C','\0'}) );
  }
}

TEST_CASE("FileOffsetChanges")
{
  using Elf::FileOffsetChanges;
  using Elf::DynamicSection;
  using Elf::Class;

  FileOffsetChanges changes;
  DynamicSection dynamicSection;

  dynamicSection.addEntry( makeStringTableSizeEntry(1) );

  SECTION("strating from a section without RUNPATH")
  {
    changes.setOriginalSizes(dynamicSection, Class::Class64);

    SECTION("setting no changes")
    {
      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == 0 );
    }

    SECTION("add the RUNPATH")
    {
      dynamicSection.setRunPath( QLatin1String("/tmp") );

      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == 16 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == 4+1 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == 21 );
    }
  }

  SECTION("starting from a section with RUNPATH")
  {
    dynamicSection.setRunPath( QLatin1String("/tmp") );
    changes.setOriginalSizes(dynamicSection, Class::Class64);

    SECTION("setting no changes")
    {
      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == 0 );
    }

    SECTION("change RUNPATH to a shorter string")
    {
      dynamicSection.setRunPath( QLatin1String("/") );

      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == -3 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == -3 );
    }

    SECTION("change RUNPATH to a longer string")
    {
      dynamicSection.setRunPath( QLatin1String("/tmp/a") );

      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == 0 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == 2 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == 2 );
    }

    SECTION("remove RUNPATH")
    {
      dynamicSection.removeRunPath();

      REQUIRE( changes.dynamicSectionChangesOffset(dynamicSection) == -16 );
      REQUIRE( changes.dynamicStringTableChangesOffset(dynamicSection) == -5 );
      REQUIRE( changes.globalChangesOffset(dynamicSection) == -21 );
    }
  }
}

TEST_CASE("shiftBytesToBegin")
{
  using Elf::shiftBytesToBegin;

  ByteArraySpan map;

  SECTION("{A} -> {}")
  {
    uchar charArray[1] = {'A'};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToBegin(map, 0, 1);
    REQUIRE( map.size == 0 );
  }

  SECTION("{s,t,r,0,o,l,d,0,1,2,3} -> {s,t,r,0,1,2,3}")
  {
    uchar charArray[11] = {'s','t','r','\0','o','l','d','\0','1','2','3'};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToBegin(map, 8, 4);
    REQUIRE( arraysAreEqual(map, {'s','t','r','\0','1','2','3'}) );
  }
}

TEST_CASE("shiftBytesToEnd")
{
  using Elf::shiftBytesToEnd;

  ByteArraySpan map;

  SECTION("{A} -> {?}")
  {
    uchar charArray[1] = {'A'};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToEnd(map, 0, 1);
    REQUIRE( map.size == 1 );
  }

  SECTION("{A,0} -> {?,A}")
  {
    uchar charArray[2] = {'A',0};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToEnd(map, 0, 1);
    REQUIRE( map.size == 2 );
    REQUIRE( map.data[1] == 'A' );
  }

  SECTION("{A,B,0} -> {?,A,B}")
  {
    uchar charArray[3] = {'A','B',0};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToEnd(map, 0, 1);
    REQUIRE( map.size == 3 );
    REQUIRE( map.data[1] == 'A' );
    REQUIRE( map.data[2] == 'B' );
  }

  SECTION("{s,t,r,0,1,2,3,0,0,0} -> {s,t,r,0,?,?,?,1,2,3}")
  {
    uchar charArray[10] = {'s','t','r','\0','1','2','3',0,0,0};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    shiftBytesToEnd(map, 4, 3);
    REQUIRE( map.size == 10 );
    REQUIRE( arraysAreEqual(map.subSpan(0, 4), {'s','t','r','\0'}) );
    REQUIRE( arraysAreEqual(map.subSpan(7, 3), {'1','2','3'}) );
  }
}

TEST_CASE("replaceBytes")
{
  using Elf::replaceBytes;
  using Elf::OffsetRange;

  ByteArraySpan map;
  OffsetRange range;

  SECTION("{A} -> {0}")
  {
    uchar charArray[1] = {'A'};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    range = OffsetRange::fromBeginAndEndOffsets(0, 1);
    replaceBytes(map, range, '\0');

    REQUIRE( map.size == 1 );
    REQUIRE( map.data[0] == '\0' );
    // MSVC does not like below version (C2668)
//     REQUIRE( arraysAreEqual(map, {'\0'}) );
  }

  SECTION("{A,B,C} -> {A,B,0}")
  {
    uchar charArray[3] = {'A','B','C'};
    map = arraySpanFromArray( charArray, sizeof(charArray) );

    range = OffsetRange::fromBeginAndEndOffsets(2, 3);
    replaceBytes(map, range, '\0');

    REQUIRE( arraysAreEqual(map, {'A','B','\0'}) );
  }
}

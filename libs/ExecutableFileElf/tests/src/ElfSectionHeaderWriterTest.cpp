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
#include "Mdt/ExecutableFile/Elf/SectionHeaderWriter.h"
#include <QString>
#include <algorithm>
#include <cassert>
#include <iostream>

using Mdt::ExecutableFile::ByteArraySpan;
using Mdt::ExecutableFile::Elf::SectionHeader;
using Mdt::ExecutableFile::Elf::FileHeader;


TEST_CASE("sectionHeaderToArray")
{
  using Mdt::ExecutableFile::Elf::sectionHeaderToArray;

  SectionHeader sectionHeader;
  FileHeader fileHeader;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  SECTION("32-bit big-endian")
  {
    sectionHeader.nameIndex = 0x12345678;
    sectionHeader.type = 0x23456789;
    sectionHeader.flags = 0x34567890;
    sectionHeader.addr = 0x45678901;
    sectionHeader.offset = 0x56789012;
    sectionHeader.size = 0x67890123;
    sectionHeader.link = 0x78901234;
    sectionHeader.info = 0x89012345;
    sectionHeader.addralign = 0x90123456;
    sectionHeader.entsize = 0x01234567;

    uchar expectedArrayData[40] = {
      // sh_name
      0x12,0x34,0x56,0x78, // 0x12345678
      // sh_type
      0x23,0x45,0x67,0x89, // 0x23456789
      // sh_flags
      0x34,0x56,0x78,0x90, // 0x34567890
      // sh_addr
      0x45,0x67,0x89,0x01, // 0x45678901
      // sh_offset
      0x56,0x78,0x90,0x12, // 0x56789012
      // sh_size
      0x67,0x89,0x01,0x23, // 0x67890123
      // sh_link
      0x78,0x90,0x12,0x34, // 0x78901234
      // sh_info
      0x89,0x01,0x23,0x45, // 0x89012345
      // sh_addralign
      0x90,0x12,0x34,0x56, // 0x90123456
      // sh_entsize
      0x01,0x23,0x45,0x67  // 0x01234567
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    fileHeader = make32BitBigEndianFileHeader();
    uchar arrayData[40] = {0};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    sectionHeaderToArray(array, sectionHeader, fileHeader);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }

  SECTION("64-bit little-endian")
  {
    sectionHeader.nameIndex = 0x12345678;
    sectionHeader.type = 0x23456789;
    sectionHeader.flags = 0x34567890;
    sectionHeader.addr = 0x45678901;
    sectionHeader.offset = 0x56789012;
    sectionHeader.size = 0x67890123;
    sectionHeader.link = 0x78901234;
    sectionHeader.info = 0x89012345;
    sectionHeader.addralign = 0x90123456;
    sectionHeader.entsize = 0x01234567;

    uchar expectedArrayData[64] = {
      // sh_name
      0x78,0x56,0x34,0x12,  // 0x12345678
      // sh_type
      0x89,0x67,0x45,0x23,  // 0x23456789
      // sh_flags
      0x90,0x78,0x56,0x34,0,0,0,0,  // 0x34567890
      // sh_addr
      0x01,0x89,0x67,0x45,0,0,0,0,  // 0x45678901
      // sh_offset
      0x12,0x90,0x78,0x56,0,0,0,0,  // 0x56789012
      // sh_size
      0x23,0x01,0x89,0x67,0,0,0,0,  // 0x67890123
      // sh_link
      0x34,0x12,0x90,0x78,  // 0x78901234
      // sh_info
      0x45,0x23,0x01,0x89,  // 0x89012345
      // sh_addralign
      0x56,0x34,0x12,0x90,0,0,0,0,  // 0x90123456
      // sh_entsize
      0x67,0x45,0x23,0x01,0,0,0,0   // 0x01234567
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    fileHeader = make64BitLittleEndianFileHeader();
    uchar arrayData[64] = {0};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    sectionHeaderToArray(array, sectionHeader, fileHeader);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }
}

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
#include "Mdt/ExecutableFile/Elf/NoteSectionReader.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionWriter.h"

using namespace Mdt::ExecutableFile::Elf;
using Mdt::ExecutableFile::ByteArraySpan;

TEST_CASE("noteSectionFromArray")
{
  NoteSection section;
  FileHeader fileHeader;
  ByteArraySpan array;

  SECTION("32-bit big-endian")
  {
    SECTION("name size is 0 (indicates a corrupted section)")
    {
      uchar arrayData[20] = {
        // name size
        0,0,0,0,  // 0
        // description size
        0,0,0,0,  // 0
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0
      };
      fileHeader = make32BitBigEndianFileHeader();
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      REQUIRE_THROWS_AS( section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident), NoteSectionReadError );
    }

    SECTION("name size is to large (indicates a corrupted section)")
    {
      uchar arrayData[20] = {
        // name size
        5,0,0,0,  // 0
        // description size
        0,0,0,0,  // 0
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0
      };
      fileHeader = make32BitBigEndianFileHeader();
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      REQUIRE_THROWS_AS( section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident), NoteSectionReadError );
    }

    SECTION("description size is to large (indicates a corrupted section)")
    {
      uchar arrayData[20] = {
        // name size
        0,0,0,5,
        // description size
        6,0,0,0,
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0
      };
      fileHeader = make32BitBigEndianFileHeader();
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      REQUIRE_THROWS_AS( section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident), NoteSectionReadError );
    }
  }
}

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
    SECTION("note without description")
    {
      uchar arrayData[20] = {
        // name size
        0,0,0,5,  // 5
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

      section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident);

      REQUIRE( section.descriptionSize == 0 );
      REQUIRE( section.type == 0x12345678 );
      REQUIRE( section.name == "Name" );
      REQUIRE( section.description.empty() );
    }

    SECTION("note with description")
    {
      uchar arrayData[28] = {
        // name size
        0,0,0,5,  // 5
        // description size
        0,0,0,6,  // 6
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0, 
        // descrition
        0x23,0x45,0x67,0x89,  // 0x23456789 (word 0)
        0,0,0x01,0x23         // 0x0123     (word 1)
      };
      fileHeader = make32BitBigEndianFileHeader();
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident);

      REQUIRE( section.descriptionSize == 6 );
      REQUIRE( section.type == 0x12345678 );
      REQUIRE( section.name == "Name" );
      REQUIRE( section.description.size() == 2 );
      REQUIRE( section.description[0] == 0x23456789 );
      REQUIRE( section.description[1] == 0x0123 );
    }
  }

  SECTION("64-bit little-endian")
  {
    fileHeader = make64BitLittleEndianFileHeader();

    SECTION("note with description")
    {
      uchar arrayData[28] = {
        // name size
        5,0,0,0,  // 5
        // description size
        6,0,0,0,  // 6
        // type
        0x78,0x56,0x34,0x12,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0, 
        // descrition
        0x89,0x67,0x45,0x23,  // 0x23456789 (word 0)
        0x23,0x01,0,0         // 0x0123     (word 1)
      };
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      section = NoteSectionReader::noteSectionFromArray(array, fileHeader.ident);

      REQUIRE( section.descriptionSize == 6 );
      REQUIRE( section.type == 0x12345678 );
      REQUIRE( section.name == "Name" );
      REQUIRE( section.description.size() == 2 );
      REQUIRE( section.description[0] == 0x23456789 );
      REQUIRE( section.description[1] == 0x0123 );
    }
  }
}

TEST_CASE("setNoteSectionToArray")
{
  NoteSection section;
  FileHeader fileHeader;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  SECTION("32-bit big-endian")
  {
    fileHeader = make32BitBigEndianFileHeader();

    SECTION("note without description")
    {
      section.descriptionSize = 0;
      section.type = 0x12345678;
      section.name = "Name";

      uchar expectedArrayData[20] = {
        // name size
        0,0,0,5,  // 5
        // description size
        0,0,0,0,  // 0
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[sizeof(expectedArrayData)];
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      NoteSectionWriter::setNoteSectionToArray(array, section, fileHeader.ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }

    SECTION("note with description")
    {
      section.descriptionSize = 6;
      section.type = 0x12345678;
      section.name = "Name";
      section.description = {0x23456789,0x0123};

      uchar expectedArrayData[28] = {
        // name size
        0,0,0,5,  // 5
        // description size
        0,0,0,6,  // 6
        // type
        0x12,0x34,0x56,0x78,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0, 
        // descrition
        0x23,0x45,0x67,0x89,  // 0x23456789 (word 0)
        0,0,0x01,0x23         // 0x0123     (word 1)
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[sizeof(expectedArrayData)];
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      NoteSectionWriter::setNoteSectionToArray(array, section, fileHeader.ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }
  }

  SECTION("64-bit little-endian")
  {
    fileHeader = make64BitLittleEndianFileHeader();

    SECTION("note with description")
    {
      section.descriptionSize = 6;
      section.type = 0x12345678;
      section.name = "Name";
      section.description = {0x23456789,0x0123};

      uchar expectedArrayData[28] = {
        // name size
        5,0,0,0,  // 5
        // description size
        6,0,0,0,  // 6
        // type
        0x78,0x56,0x34,0x12,  // 0x12345678
        // name
        'N','a','m','e',  // Name
        '\0',0,0,0, 
        // descrition
        0x89,0x67,0x45,0x23,  // 0x23456789 (word 0)
        0x23,0x01,0,0         // 0x0123     (word 1)
      };
      expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

      uchar arrayData[sizeof(expectedArrayData)];
      array = arraySpanFromArray( arrayData, sizeof(arrayData) );

      NoteSectionWriter::setNoteSectionToArray(array, section, fileHeader.ident);
      REQUIRE( arraysAreEqual(array, expectedArray) );
    }
  }
}

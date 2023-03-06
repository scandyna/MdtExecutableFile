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
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"

using Mdt::ExecutableFile::Elf::SectionHeader;
using Mdt::ExecutableFile::Elf::SectionType;

TEST_CASE("sectionType")
{
  SectionHeader header;

  SECTION("Null")
  {
    header.type = 0;
    REQUIRE( header.sectionType() == SectionType::Null );
  }

  SECTION("StringTable")
  {
    header.type = 3;
    REQUIRE( header.sectionType() == SectionType::StringTable );
  }
}

TEST_CASE("SectionAttributeFlags")
{
  SectionHeader header;

  SECTION("Alloc")
  {
    header.flags = 0x02;

    REQUIRE( header.allocatesMemory() );
    REQUIRE( !header.holdsTls() );
  }

  SECTION("Write")
  {
    SECTION("0x01 writable")
    {
      header.flags = 0x01;

      REQUIRE( header.isWritable() );
    }

    SECTION("0x02 alloc")
    {
      header.flags = 0x02;

      REQUIRE( !header.isWritable() );
    }

    SECTION("0x03 write and alloc")
    {
      header.flags = 0x03;

      REQUIRE( header.isWritable() );
    }

    SECTION("0x04 executable instruction")
    {
      header.flags = 0x04;

      REQUIRE( !header.isWritable() );
    }
  }
}

TEST_CASE("fileOffsetEnd")
{
  SectionHeader header;
  header.offset = 10;
  header.size = 5;

  REQUIRE( header.fileOffsetEnd() == 15 );
}

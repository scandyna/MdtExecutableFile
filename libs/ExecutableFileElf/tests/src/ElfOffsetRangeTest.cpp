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
#include "Mdt/ExecutableFile/Elf/OffsetRange.h"

using Mdt::ExecutableFile::Elf::OffsetRange;
using Mdt::ExecutableFile::Elf::ProgramHeader;
using Mdt::ExecutableFile::Elf::SectionHeader;

TEST_CASE("default constructed")
{
  OffsetRange range;

  REQUIRE( range.begin() == 0 );
  REQUIRE( range.end() == 0 );
  REQUIRE( range.byteCount() == 0 );
  REQUIRE( range.isEmpty() );
}

TEST_CASE("minimumSizeToAccessRange")
{
  OffsetRange range;

  SECTION("Null range")
  {
    range = OffsetRange::fromBeginAndEndOffsets(0, 0);

    REQUIRE( range.minimumSizeToAccessRange() == 0 );
  }

  SECTION("range 10-11")
  {
    range = OffsetRange::fromBeginAndEndOffsets(10, 12);

    REQUIRE( range.lastOffset() == 11 );
    REQUIRE( range.minimumSizeToAccessRange() == 12 );
  }
}

TEST_CASE("fromBeginAndEndOffsets")
{
  OffsetRange range;

  SECTION("Null range")
  {
    range = OffsetRange::fromBeginAndEndOffsets(0, 0);

    REQUIRE( range.isEmpty() );
  }

  SECTION("1 byte range")
  {
    range = OffsetRange::fromBeginAndEndOffsets(10, 11);

    REQUIRE( range.begin() == 10 );
    REQUIRE( range.byteCount() == 1 );
    REQUIRE( !range.isEmpty() );
    REQUIRE( range.lastOffset() == 10 );
    REQUIRE( range.end() == 11 );
  }
}

TEST_CASE("fromProgrameHeader")
{
  OffsetRange range;
  ProgramHeader header;

  SECTION("null segment")
  {
    header.offset = 0;
    header.filesz = 0;

    range = OffsetRange::fromProgrameHeader(header);

    REQUIRE( range.isEmpty() );
  }

  SECTION("1 byte segment")
  {
    header.offset = 10;
    header.filesz = 1;

    range = OffsetRange::fromProgrameHeader(header);

    REQUIRE( range.begin() == 10 );
    REQUIRE( range.byteCount() == 1 );
    REQUIRE( !range.isEmpty() );
    REQUIRE( range.lastOffset() == 10 );
    REQUIRE( range.end() == 11 );
  }
}

TEST_CASE("fromSectionHeader")
{
  OffsetRange range;
  SectionHeader header;

  SECTION("1 byte section")
  {
    header.offset = 10;
    header.size = 1;

    range = OffsetRange::fromSectionHeader(header);

    REQUIRE( range.begin() == 10 );
    REQUIRE( range.byteCount() == 1 );
    REQUIRE( !range.isEmpty() );
    REQUIRE( range.lastOffset() == 10 );
    REQUIRE( range.end() == 11 );
  }
}

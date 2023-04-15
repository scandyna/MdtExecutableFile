// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "SectionGraphicsItemData.h"
#include <QLatin1String>

using Mdt::ExecutableFile::Elf::SectionHeader;
using Catch::Matchers::WithinRel;


TEST_CASE("offset")
{
  SectionGraphicsItemData data;

  SECTION("by default offset is 0")
  {
    REQUIRE( data.offset() == 0 );
  }

  SECTION("set offset 1000")
  {
    data.setOffset(1000);

    REQUIRE( data.offset() == 1000 );
    REQUIRE_THAT( data.offsetF(), WithinRel(1000.0) );
  }
}

TEST_CASE("size")
{
  SectionGraphicsItemData data;

  SECTION("by default size is 0")
  {
    REQUIRE( data.size() == 0 );
  }

  SECTION("set size 100")
  {
    data.setSize(100);

    REQUIRE( data.size() == 100 );
    REQUIRE_THAT( data.sizeF(), WithinRel(100.0) );
  }
}

TEST_CASE("lastAddress")
{
  SectionGraphicsItemData data;

  SECTION("by default, last address is 0")
  {
    REQUIRE( data.lastAddress() == 0 );
  }

  SECTION("offset 10 and default size (0)")
  {
    data.setOffset(10);

    REQUIRE( data.lastAddress() == 10 );
  }

  SECTION("offset 10 and size 1")
  {
    data.setOffset(10);
    data.setSize(1);

    REQUIRE( data.lastAddress() == 10 );
  }

  SECTION("offset 10 and size 2")
  {
    data.setOffset(10);
    data.setSize(2);

    REQUIRE( data.lastAddress() == 11 );
  }
}

TEST_CASE("name")
{
  SectionGraphicsItemData data;

  data.setName(".dynstr");

  REQUIRE( data.name() == QLatin1String(".dynstr") );
}

TEST_CASE("fromSectionHeader")
{
  SectionHeader header;
  header.offset = 20;
  header.size = 3;
  header.name = ".dynamic";

  const auto data = SectionGraphicsItemData::fromSectionHeader(header);

  REQUIRE( data.offset() == 20 );
  REQUIRE( data.size() == 3 );
  REQUIRE( data.name() == QLatin1String(".dynamic") );
}

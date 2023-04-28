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
#include "LayoutViewSegmentLayout.h"

using Mdt::ExecutableFile::Elf::ProgramHeader;
using Catch::Matchers::WithinRel;


TEST_CASE("fromHeaderAndHeight")
{
  ProgramHeader header;
  header.offset = 100;
  header.filesz = 12;

  const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

  REQUIRE( item.offset() == 100 );
  REQUIRE_THAT( item.offsetF(), WithinRel(100.0) );
  REQUIRE( item.lastAddress() == 111 );
  REQUIRE_THAT( item.heightF(), WithinRel(20.0) );
}

TEST_CASE("lastAddress")
{
  ProgramHeader header;
  header.offset = 100;

  SECTION("size 0")
  {
    header.filesz = 0;

    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( item.lastAddress() == 100 );
  }

  SECTION("size 1")
  {
    header.filesz = 1;

    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( item.lastAddress() == 100 );
  }

  SECTION("size 2")
  {
    header.filesz = 2;

    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( item.lastAddress() == 101 );
  }
}

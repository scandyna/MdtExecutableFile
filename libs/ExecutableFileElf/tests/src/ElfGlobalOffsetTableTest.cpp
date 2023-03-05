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
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTable.h"

using namespace Mdt::ExecutableFile::Elf;

TEST_CASE("DynamicSectionAddress")
{
  GlobalOffsetTable table;
  GlobalOffsetTableEntry entry;

  SECTION("empty table")
  {
    REQUIRE( !table.containsDynamicSectionAddress() );
  }

  SECTION("table contains null bytes at first entry (which should be address to _DYNAMIC)")
  {
    entry.data = 0;
    table.addEntryFromFile(entry);

    REQUIRE( !table.containsDynamicSectionAddress() );
  }

  SECTION("table contains the dynamic section address")
  {
    entry.data = 1234;
    table.addEntryFromFile(entry);

    REQUIRE( table.containsDynamicSectionAddress() );
    REQUIRE( table.dynamicSectionAddress() == 1234 );

    SECTION("change the address")
    {
      table.setDynamicSectionAddress(25);

      REQUIRE( table.dynamicSectionAddress() == 25 );
    }
  }
}

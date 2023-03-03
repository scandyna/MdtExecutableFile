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
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/Elf/StringTable.h"

using namespace Mdt::ExecutableFile;
using Mdt::ExecutableFile::Elf::StringTable;
using Mdt::ExecutableFile::ByteArraySpan;
using Mdt::ExecutableFile::StringTableError;

TEST_CASE("fromCharArray")
{
  StringTable table;
  ByteArraySpan charArraySpan;

  SECTION("1 not null char")
  {
    uchar charArray[1] = {1};
    charArraySpan = arraySpanFromArray( charArray, sizeof(charArray) );
    REQUIRE_THROWS_AS(StringTable::fromCharArray(charArraySpan), StringTableError);
  }

  SECTION("name. not null terminated")
  {
    uchar charArray[6] = {'\0','n','a','m','e','.'};
    charArraySpan = arraySpanFromArray( charArray, sizeof(charArray) );
    REQUIRE_THROWS_AS(StringTable::fromCharArray(charArraySpan), StringTableError);
  }

}

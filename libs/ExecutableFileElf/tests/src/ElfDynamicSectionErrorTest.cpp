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
#include "ElfDynamicSectionTestCommon.h"

using Mdt::ExecutableFile::ExecutableFileReadError;

TEST_CASE("getSoName")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[8] = {'\0','S','o','N','a','m','e','\0'};
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("SoName string table index out of bound")
  {
    section.addEntry( makeSoNameEntry(8) );
    REQUIRE_THROWS_AS( section.getSoName(), ExecutableFileReadError );
  }
}

TEST_CASE("getNeededSharedLibraries")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[17] = {
    '\0',
    'l','i','b','A','.','s','o','\0',
    'l','i','b','B','.','s','o','\0'
  };
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("libA.so string table index out of bound")
  {
    section.addEntry( makeNeededEntry(35) );
    REQUIRE_THROWS_AS( section.getNeededSharedLibraries(), ExecutableFileReadError );
  }
}

TEST_CASE("getRunPath string table index out of bound")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[13] = {
    '\0',
    '/','t','m','p',':',
    '/','p','a','t','h','2','\0'
  };
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("/tmp:/path2")
  {
    section.addEntry( makeRunPathEntry(47) );
    REQUIRE_THROWS_AS( section.getRunPath(), ExecutableFileReadError );
  }
}

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
#include "Mdt/ExecutableFile/Elf/NoteSection.h"

using namespace Mdt::ExecutableFile::Elf;

TEST_CASE("byteCountAligned")
{
  NoteSection section;

  SECTION("default constructed")
  {
    REQUIRE( section.isNull() );
  }

  SECTION("note contains a name and type")
  {
    /*
     * name size: 4 bytes
     * description size: 4 bytes
     * type: 4 bytes
     * name: N,a,m,e,\0 -> 5 bytes -> 8 bytes
     * description: 0 byte
     */
    section.descriptionSize = 0;
    section.type = 1;
    section.name = "Name";

    REQUIRE( section.byteCountAligned() == 20 );
  }

  SECTION("GNU .note.ABI-tag")
  {
    /*
     * name size: 4 bytes
     * description size: 4 bytes
     * type: 4 bytes
     * name: G,N,U,\0 -> 4 bytes
     * description: 16 (0x10) bytes
     */
    section.descriptionSize = 0;
    section.type = 1;
    section.name = "GNU";
    section.description = {1,2,3,4};

    REQUIRE( section.byteCountAligned() == 32 );
  }
}

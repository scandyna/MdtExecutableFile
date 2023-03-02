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
#include "ElfSectionHeaderTestUtils.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionTable.h"
#include <string>

using namespace Mdt::ExecutableFile::Elf;

NoteSection makeNoteSection(const std::string & name)
{
  NoteSection section;

  section.name = name;

  return section;
}

TEST_CASE("updateSectionHeaders")
{
  NoteSectionTable noteSectionTable;
  std::vector<SectionHeader> sectionHeaderTable;

  SECTION("both tables are empty")
  {
    noteSectionTable.updateSectionHeaders(sectionHeaderTable);
  }

  SECTION("section header table is empty")
  {
    noteSectionTable.addSectionFromFile( makeNoteSectionHeader(".note.A"), makeNoteSection("A") );

    noteSectionTable.updateSectionHeaders(sectionHeaderTable);

    REQUIRE( noteSectionTable.sectionCount() == 1 );
  }

  SECTION("section header table contains no note section")
  {
    sectionHeaderTable.emplace_back( makeNullSectionHeader() );

    noteSectionTable.addSectionFromFile( makeNoteSectionHeader(".note.A"), makeNoteSection("A") );

    noteSectionTable.updateSectionHeaders(sectionHeaderTable);

    REQUIRE( noteSectionTable.sectionCount() == 1 );
    REQUIRE( noteSectionTable.sectionHeaderAt(0).name == ".note.A" );
  }

  SECTION("section header table contains a note section, but does not match any one in the note section table")
  {
    sectionHeaderTable.emplace_back( makeNoteSectionHeader(".note") );

    noteSectionTable.addSectionFromFile( makeNoteSectionHeader(".note.A"), makeNoteSection("A") );

    noteSectionTable.updateSectionHeaders(sectionHeaderTable);

    REQUIRE( noteSectionTable.sectionCount() == 1 );
    REQUIRE( noteSectionTable.sectionHeaderAt(0).name == ".note.A" );
  }

  SECTION("section header table contains a note section that matches one in the note section table")
  {
    SectionHeader noteSectionHeaderA = makeNoteSectionHeader(".note.A");
    noteSectionHeaderA.offset = 256;

    sectionHeaderTable.emplace_back( makeNullSectionHeader() );
    sectionHeaderTable.emplace_back(noteSectionHeaderA);

    noteSectionTable.addSectionFromFile( noteSectionHeaderA, makeNoteSection("A") );

    sectionHeaderTable[1].offset = 584;

    noteSectionTable.updateSectionHeaders(sectionHeaderTable);

    REQUIRE( noteSectionTable.sectionCount() == 1 );
    REQUIRE( noteSectionTable.sectionHeaderAt(0).offset == 584 );
  }
}

TEST_CASE("findMinimumSizeToWriteTable")
{
  NoteSectionTable noteSectionTable;

  SECTION("empty table")
  {
    REQUIRE( noteSectionTable.findMinimumSizeToWriteTable() == 0 );
  }

  SECTION("1 note section")
  {
    SectionHeader headerA = makeNoteSectionHeader(".note.A");
    headerA.offset = 25;
    headerA.size = 10;

    noteSectionTable.addSectionFromFile( headerA, makeNoteSection("A") );

    REQUIRE( noteSectionTable.findMinimumSizeToWriteTable() == 35 );
  }

  SECTION("2 note sections")
  {
    SectionHeader headerA = makeNoteSectionHeader(".note.A");
    headerA.offset = 25;
    headerA.size = 10;

    SectionHeader headerB = makeNoteSectionHeader(".note.B");
    headerA.offset = 50;
    headerA.size = 15;

    noteSectionTable.addSectionFromFile( headerA, makeNoteSection("A") );
    noteSectionTable.addSectionFromFile( headerB, makeNoteSection("B") );

    REQUIRE( noteSectionTable.findMinimumSizeToWriteTable() == 65 );
  }
}

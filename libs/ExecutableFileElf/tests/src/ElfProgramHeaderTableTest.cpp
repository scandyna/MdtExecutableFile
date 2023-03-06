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
#include "ElfProgramHeaderTestUtils.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderTable.h"

using Mdt::ExecutableFile::Elf::SegmentType;
using Mdt::ExecutableFile::Elf::ProgramHeader;
using Mdt::ExecutableFile::Elf::ProgramHeaderTable;

TEST_CASE("ProgramHeaderTableHeader_PT_PHDR")
{
  ProgramHeaderTable table;

  SECTION("default constructed")
  {
    REQUIRE( !table.containsProgramHeaderTableHeader() );
  }

  SECTION("add PT_PHDR")
  {
    table.addHeaderFromFile( makeProgramHeaderTableProgramHeader() );
    REQUIRE( table.containsProgramHeaderTableHeader() );
    REQUIRE( table.programHeaderTableHeader().segmentType() == SegmentType::ProgramHeaderTable );
  }
}

TEST_CASE("addHeader")
{
  ProgramHeaderTable table;

  SECTION("table does not contain the program header table (PT_PHDR)")
  {
    ProgramHeader dynamicSectionHeader = makeDynamicSectionProgramHeader();

    table.addHeader(dynamicSectionHeader, 50);

    REQUIRE( table.headerCount() == 1 );
    REQUIRE( table.containsDynamicSectionHeader() );
  }

  SECTION("table contains the program header table (PT_PHDR)")
  {
    ProgramHeader programHeaderTableHeader = makeProgramHeaderTableProgramHeader();
    ProgramHeader dynamicSectionHeader = makeDynamicSectionProgramHeader();

    table.addHeader(programHeaderTableHeader, 50);
    table.addHeader(dynamicSectionHeader, 50);

    REQUIRE( table.headerCount() == 2 );
    REQUIRE( table.containsProgramHeaderTableHeader() );
    REQUIRE( table.programHeaderTableHeader().memsz == 100 );
    REQUIRE( table.programHeaderTableHeader().filesz == 100 );
    REQUIRE( table.containsDynamicSectionHeader() );
  }
}

TEST_CASE("DynamicSection")
{
  ProgramHeaderTable table;
  ProgramHeader dynamicSectionHeader;

  SECTION("default constructed")
  {
    REQUIRE( !table.containsDynamicSectionHeader() );
  }

  SECTION("add dynamic section to table")
  {
    dynamicSectionHeader = makeDynamicSectionProgramHeader();
    dynamicSectionHeader.offset = 100;

    table.addHeaderFromFile(dynamicSectionHeader);

    REQUIRE( table.containsDynamicSectionHeader() );
    REQUIRE( table.dynamicSectionHeader().offset == 100 );
  }

  SECTION("add other segment header")
  {
    table.addHeaderFromFile( makeNullProgramHeader() );
    REQUIRE( !table.containsDynamicSectionHeader() );
  }
}

TEST_CASE("programInterpreterSection")
{
  ProgramHeaderTable table;
  ProgramHeader interpHeader;

  SECTION("default constructed")
  {
    REQUIRE( !table.containsProgramInterpreterProgramHeader() );
  }

  SECTION("add the PT_INTERP header")
  {
    interpHeader = makeProgramInterpreterProgramHeader();
    interpHeader.offset = 150;

    table.addHeaderFromFile(interpHeader);

    REQUIRE( table.containsProgramInterpreterProgramHeader() );
    REQUIRE( table.programInterpreterProgramHeader().offset == 150 );
  }

  SECTION("add a other program header")
  {
    table.addHeaderFromFile( makeNullProgramHeader() );
    REQUIRE( !table.containsProgramInterpreterProgramHeader() );
  }
}

TEST_CASE("notesSegment")
{
  ProgramHeaderTable table;
  ProgramHeader noteHeader;

  SECTION("default constructed")
  {
    REQUIRE( !table.containsNoteProgramHeader() );
  }

  SECTION("add the PT_INTERP header")
  {
    noteHeader = makeNoteProgramHeader();
    noteHeader.offset = 160;

    table.addHeaderFromFile(noteHeader);

    REQUIRE( table.containsNoteProgramHeader() );
    REQUIRE( table.noteProgramHeader().offset == 160 );
  }

  SECTION("add a other program header")
  {
    table.addHeaderFromFile( makeNullProgramHeader() );
    REQUIRE( !table.containsNoteProgramHeader() );
  }
}

TEST_CASE("gnuRelRoHeader")
{
  ProgramHeaderTable table;
  ProgramHeader gnuRelRoHeader;

  SECTION("default constructed")
  {
    REQUIRE( !table.containsGnuRelRoHeader() );
  }

  SECTION("add the PT_GNU_RELRO header")
  {
    gnuRelRoHeader = makeGnuRelRoProgramHeader();
    gnuRelRoHeader.offset = 150;
    gnuRelRoHeader.memsz = 10;
    gnuRelRoHeader.filesz = 10;

    table.addHeaderFromFile(gnuRelRoHeader);

    REQUIRE( table.containsGnuRelRoHeader() );
    REQUIRE( table.gnuRelRoHeader().offset == 150 );

    table.setGnuRelRoHeaderSize(25);
    REQUIRE( table.gnuRelRoHeader().memsz == 25 );
    REQUIRE( table.gnuRelRoHeader().filesz == 25 );
  }

  SECTION("add a other program header")
  {
    table.addHeaderFromFile( makeNullProgramHeader() );
    REQUIRE( !table.containsGnuRelRoHeader() );
  }
}

TEST_CASE("findLastSegmentVirtualAddressEnd")
{
  ProgramHeaderTable table;

  ProgramHeader dynamicSectionHeader = makeDynamicSectionProgramHeader();
  dynamicSectionHeader.vaddr = 1000;
  dynamicSectionHeader.memsz = 100;

  table.addHeaderFromFile(dynamicSectionHeader);

  REQUIRE( table.findLastSegmentVirtualAddressEnd() == 1100 );
}

TEST_CASE("findLastSegmentFileOffsetEnd")
{
  ProgramHeaderTable table;

  ProgramHeader dynamicSectionHeader = makeDynamicSectionProgramHeader();
  dynamicSectionHeader.offset = 1000;
  dynamicSectionHeader.filesz = 100;

  table.addHeaderFromFile(dynamicSectionHeader);

  REQUIRE( table.findLastSegmentFileOffsetEnd() == 1100 );
}

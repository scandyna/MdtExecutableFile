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
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"

using Mdt::ExecutableFile::Elf::ProgramHeader;
using Mdt::ExecutableFile::Elf::SegmentType;
using Mdt::ExecutableFile::Elf::SegmentPermission;

TEST_CASE("segmentType")
{
  ProgramHeader header;

  SECTION("PT_NULL")
  {
    header.type = 0;
    REQUIRE( header.segmentType() == SegmentType::Null );
  }

  SECTION("PT_LOAD")
  {
    header.type = 1;
    REQUIRE( header.segmentType() == SegmentType::Load );
  }

  SECTION("PT_DYNAMIC")
  {
    header.type = 2;
    REQUIRE( header.segmentType() == SegmentType::Dynamic );
  }

  SECTION("PT_INTERP")
  {
    header.type = 3;
    REQUIRE( header.segmentType() == SegmentType::Interpreter );
  }

  SECTION("PT_NOTE")
  {
    header.type = 4;
    REQUIRE( header.segmentType() == SegmentType::Note );
  }

  SECTION("PT_PHDR")
  {
    header.type = 6;
    REQUIRE( header.segmentType() == SegmentType::ProgramHeaderTable );
  }

  SECTION("PT_TLS")
  {
    header.type = 7;
    REQUIRE( header.segmentType() == SegmentType::Tls );
  }

  SECTION("PT_GNU_EH_FRAME")
  {
    header.type = 0x6474e550;
    REQUIRE( header.segmentType() == SegmentType::GnuEhFrame );
  }

  SECTION("PT_GNU_STACK")
  {
    header.type = 0x6474e551;
    REQUIRE( header.segmentType() == SegmentType::GnuStack );
  }

  SECTION("PT_GNU_RELRO")
  {
    header.type = 0x6474e552;
    REQUIRE( header.segmentType() == SegmentType::GnuRelRo );
  }
}

TEST_CASE("setSegmentType")
{
  ProgramHeader header;

  header.setSegmentType(SegmentType::Load);

  REQUIRE( header.segmentType() == SegmentType::Load );
}

TEST_CASE("SegmentPermission")
{
  ProgramHeader header;

  SECTION("Executable")
  {
    header.setPermissions(SegmentPermission::Execute);
    REQUIRE( header.isExecutable() );
    REQUIRE( !header.isWritable() );
    REQUIRE( !header.isReadable() );
  }

  SECTION("Readable and writable")
  {
    header.setPermissions(SegmentPermission::Read | SegmentPermission::Write);
    REQUIRE( !header.isExecutable() );
    REQUIRE( header.isWritable() );
    REQUIRE( header.isReadable() );
  }
}

TEST_CASE("requiresAlignment")
{
  ProgramHeader header;

  SECTION("align 0")
  {
    header.align = 0;
    REQUIRE( !header.requiresAlignment() );
  }

  SECTION("align 1")
  {
    header.align = 1;
    REQUIRE( !header.requiresAlignment() );
  }

  SECTION("align 4")
  {
    header.align = 4;
    REQUIRE( header.requiresAlignment() );
  }
}

TEST_CASE("segmentVirtualAddressEnd")
{
  ProgramHeader header;
  header.vaddr = 10;
  header.memsz = 5;

  REQUIRE( header.segmentVirtualAddressEnd() == 15 );
}

TEST_CASE("fileOffsetEnd")
{
  ProgramHeader header;
  header.offset = 10;
  header.filesz = 5;

  REQUIRE( header.fileOffsetEnd() == 15 );
}

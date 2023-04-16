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
#include "SegmentTypeString.h"
#include <QLatin1String>

using Mdt::ExecutableFile::Elf::SegmentType;


TEST_CASE("segmentTypeShortName")
{
  SECTION("Null")
  {
    REQUIRE( segmentTypeShortName(SegmentType::Null) == QLatin1String("NULL") );
  }

  SECTION("GnuRelRo")
  {
    REQUIRE( segmentTypeShortName(SegmentType::GnuRelRo) == QLatin1String("GNU_RELRO") );
  }
}

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
#include "Mdt/ExecutableFile/RPath.h"
#include "Mdt/ExecutableFile/RPathElf.h"
#include <QLatin1String>

using Mdt::ExecutableFile::RPathEntry;
using Mdt::ExecutableFile::RPath;
using Mdt::ExecutableFile::RPathElf;
using Mdt::ExecutableFile::RPathFormatError;

TEST_CASE("ELF_rPathFromString")
{
  RPath rpath;

  SECTION("$ORIGINa")
  {
    REQUIRE_THROWS_AS( rpath = RPathElf::rPathFromString( QLatin1String("$ORIGINa") ), RPathFormatError);
  }

  SECTION("${ORIGIN}a")
  {
    REQUIRE_THROWS_AS( rpath = RPathElf::rPathFromString( QLatin1String("${ORIGIN}a") ), RPathFormatError);
  }
}

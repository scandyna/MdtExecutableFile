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
#include "RPathTestUtils.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("makeRPathFromPathList")
{
  const RPath rpath = makeRPathFromPathList({".","../lib"});

  REQUIRE( rpath.entriesCount() == 2 );
  REQUIRE( rpath.entryAt(0).path() == QLatin1String(".") );
  REQUIRE( rpath.entryAt(1).path() == QLatin1String("../lib") );
}

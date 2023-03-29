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
#include "TestUtils.h"
#include "TestFileUtils.h"
#include "TestBinariesUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileReader.h"
#include <QString>

using namespace Mdt::ExecutableFile;

TEST_CASE("getRunPath")
{
  ExecutableFileReader reader;

  SECTION("shared library")
  {
    reader.openFile( testSharedLibraryFilePath());
    REQUIRE( !reader.getRunPath().isEmpty() );
    reader.close();
  }

  SECTION("dynamic linked executable")
  {
    reader.openFile( testExecutableFilePath() );
    REQUIRE( !reader.getRunPath().isEmpty() );
    reader.close();
  }
}

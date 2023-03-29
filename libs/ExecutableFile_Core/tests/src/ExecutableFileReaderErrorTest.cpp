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
#include "TestBinariesUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileReader.h"
#include "Mdt/ExecutableFile/Platform.h"
#include <QString>

using namespace Mdt::ExecutableFile;

TEST_CASE("open_wrong_platform_file")
{
  ExecutableFileReader reader;
  const QString file = testExecutableFilePath();
  const Platform platform = getNonNativePlatform();
  REQUIRE_THROWS_AS(reader.openFile(file, platform), FileOpenError);
}

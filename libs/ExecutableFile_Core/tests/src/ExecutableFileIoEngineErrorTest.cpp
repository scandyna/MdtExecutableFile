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
#include "Mdt/ExecutableFile/ExecutableFileIoEngine.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("open_wrong_platform_file")
{
  ExecutableFileIoEngine engine;
  const QString file = testExecutableFilePath();
  const Platform platform = getNonNativePlatform();
  REQUIRE_THROWS_AS(engine.openFile(file, ExecutableFileOpenMode::ReadOnly, platform), FileOpenError);
}

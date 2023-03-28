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
#include "TestBinariesUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileIoEngine.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("open_close")
{
  ExecutableFileIoEngine engine;

  SECTION("default constructed")
  {
    REQUIRE( !engine.isOpen() );
  }

  SECTION("call close on a default constructed engine does nothing (and does not crach)")
  {
    engine.close();
    REQUIRE( !engine.isOpen() );
  }

  SECTION("open a executable - read only")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isOpen() );
    engine.close();
    REQUIRE( !engine.isOpen() );
  }

  SECTION("open a native executable - read only")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly, Platform::nativePlatform() );
    REQUIRE( engine.isOpen() );
    engine.close();
    REQUIRE( !engine.isOpen() );
  }

}

TEST_CASE("getFilePlatform")
{
  ExecutableFileIoEngine engine;
  const auto platform = Platform::nativePlatform();

  SECTION("executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.getFilePlatform() == platform );
  }

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.getFilePlatform() == platform );
  }
}

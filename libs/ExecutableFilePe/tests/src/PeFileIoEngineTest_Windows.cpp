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
#include "Mdt/ExecutableFile/PeFileIoEngine.h"
#include <QString>
#include <QLatin1String>

using namespace Mdt::ExecutableFile;

TEST_CASE("isPeImageFile")
{
    PeFileIoEngine engine;

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isPeImageFile() );
  }

  SECTION("dynamic linked executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isPeImageFile() );
  }
}

TEST_CASE("isExecutableOrSharedLibrary")
{
    PeFileIoEngine engine;

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isExecutableOrSharedLibrary() );
    engine.close();
  }

  SECTION("static library")
  {
    engine.openFile( testStaticLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }

  SECTION("dynamic linked executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isExecutableOrSharedLibrary() );
    engine.close();
  }
}

TEST_CASE("getNeededSharedLibraries")
{
    PeFileIoEngine engine;
  QStringList libraries;

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    libraries = engine.getNeededSharedLibraries();
    REQUIRE( !libraries.isEmpty() );
    REQUIRE( containsQt5Core(libraries) );
    engine.close();
  }

  SECTION("dynamic linked executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    libraries = engine.getNeededSharedLibraries();
    REQUIRE( !libraries.isEmpty() );
    REQUIRE( containsTestSharedLibrary(libraries) );
    REQUIRE( containsQt5Core(libraries) );
    engine.close();
  }
}

TEST_CASE("open_2_consecutive_files_with_1_instance")
{
    PeFileIoEngine engine;

  /*
   * Open a big library first
   */

  engine.openFile( qt5CoreFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( !engine.getNeededSharedLibraries().isEmpty() );
  engine.close();

  engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( !engine.getNeededSharedLibraries().isEmpty() );
  engine.close();
}

TEST_CASE("call_many_members_on_1_instance")
{
    PeFileIoEngine engine;

  engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( engine.isExecutableOrSharedLibrary() );
  REQUIRE( !engine.getNeededSharedLibraries().isEmpty() );
  engine.close();
}

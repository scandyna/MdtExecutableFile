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

#include "TestUtils.h"
#include "TestFileUtils.h"

#include "Mdt/ExecutableFile/ElfFileIoEngine.h"
#include <QString>
#include <QLatin1String>

using namespace Mdt::ExecutableFile;

TEST_CASE("isElfFile")
{
  ElfFileIoEngine engine;

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isElfFile() );
  }

  /*
   * Would require to open the archive and inspect contents,
   * not supported now
   */
//   SECTION("static library")
//   {
//     REQUIRE( ElfFileIoEngine::isElfFile( QString::fromLocal8Bit(TEST_STATIC_LIBRARY_FILE_PATH) ) );
//   }

  SECTION("dynamic linked executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isElfFile() );
  }
}

TEST_CASE("isExecutableOrSharedLibrary")
{
  ElfFileIoEngine engine;

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
  ElfFileIoEngine engine;
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

TEST_CASE("getRunPath")
{
  ElfFileIoEngine engine;

  SECTION("shared library")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.getRunPath().isEmpty() );
    engine.close();
  }

  SECTION("dynamic linked executable")
  {
    engine.openFile( testExecutableFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.getRunPath().isEmpty() );
    engine.close();
  }
}

TEST_CASE("getSoName")
{
  ElfFileIoEngine engine;

  SECTION("libtestSharedLibrary.so")
  {
    engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.getSoName() == QLatin1String("libtestSharedLibrary.so") );
    engine.close();
  }
}

TEST_CASE("open_2_consecutive_files_with_1_instance")
{
  ElfFileIoEngine engine;

  /*
   * Open a big library first
   */

  engine.openFile( qt5CoreFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( engine.getSoName() == QLatin1String("libQt5Core.so.5") );
  engine.close();

  engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( engine.getSoName() == QLatin1String("libtestSharedLibrary.so") );
  engine.close();
}

TEST_CASE("call_many_members_one_1_instance")
{
  ElfFileIoEngine engine;

  engine.openFile( testSharedLibraryFilePath(), ExecutableFileOpenMode::ReadOnly );
  REQUIRE( engine.isExecutableOrSharedLibrary() );
  REQUIRE( engine.getSoName() == QLatin1String("libtestSharedLibrary.so") );
  REQUIRE( !engine.getNeededSharedLibraries().isEmpty() );
  REQUIRE( !engine.getRunPath().isEmpty() );
  engine.close();
}

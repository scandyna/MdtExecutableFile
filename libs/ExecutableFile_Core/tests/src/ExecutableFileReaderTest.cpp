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
#include <QTemporaryFile>

using namespace Mdt::ExecutableFile;

TEST_CASE("open_close")
{
  ExecutableFileReader reader;

  SECTION("default constructed")
  {
    REQUIRE( !reader.isOpen() );
  }

  SECTION("open a executable")
  {
    reader.openFile( testExecutableFilePath() );
    REQUIRE( reader.isOpen() );
    reader.close();
    REQUIRE( !reader.isOpen() );
  }

  SECTION("open a native executable")
  {
    reader.openFile( testExecutableFilePath(), Platform::nativePlatform() );
    REQUIRE( reader.isOpen() );
    reader.close();
    REQUIRE( !reader.isOpen() );
  }
}

TEST_CASE("getFilePlatform")
{
  ExecutableFileReader reader;
  const auto platform = Platform::nativePlatform();

  SECTION("executable")
  {
    reader.openFile( testExecutableFilePath() );
    REQUIRE( reader.getFilePlatform() == platform );
  }

  SECTION("shared library")
  {
    reader.openFile( testSharedLibraryFilePath() );
    REQUIRE( reader.getFilePlatform() == platform );
  }
}

TEST_CASE("isExecutableOrSharedLibrary")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  ExecutableFileReader reader;

  SECTION("empty file")
  {
    file.close();
    reader.openFile( file.fileName() );
    REQUIRE( !reader.isExecutableOrSharedLibrary() );
    reader.close();
  }

  SECTION("text file")
  {
    REQUIRE( writeTextFileUtf8( file, generateStringWithNChars(100) ) );
    file.close();
    reader.openFile( file.fileName() );
    REQUIRE( !reader.isExecutableOrSharedLibrary() );
    reader.close();
  }

  SECTION("executable file")
  {
    reader.openFile( testExecutableFilePath() );
    REQUIRE( reader.isExecutableOrSharedLibrary() );
    reader.close();
  }

  SECTION("shared library")
  {
    reader.openFile( testSharedLibraryFilePath() );
    REQUIRE( reader.isExecutableOrSharedLibrary() );
    reader.close();
  }

  SECTION("static library")
  {
    reader.openFile(  testStaticLibraryFilePath() );
    REQUIRE( !reader.isExecutableOrSharedLibrary() );
    reader.close();
  }
}

/*
 * MSVC allways adds debug symbols,
 * so this test will fail for Release build
 */
#ifndef COMPILER_IS_MSVC
TEST_CASE("containsDebugSymbols")
{
  ExecutableFileReader reader;
  const bool isDebug = currentBuildIsWithDebugSymbolsFlag();

  SECTION("shared library")
  {
    reader.openFile( testSharedLibraryFilePath() );
    REQUIRE( reader.containsDebugSymbols() == isDebug );
    reader.close();
  }

  SECTION("dynamic linked executable")
  {
    reader.openFile( testExecutableFilePath() );
    REQUIRE( reader.containsDebugSymbols() == isDebug );
    reader.close();
  }
}
#endif // #ifndef COMPILER_IS_MSVC

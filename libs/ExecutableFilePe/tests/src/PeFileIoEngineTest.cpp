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
#include "Mdt/ExecutableFile/PeFileIoEngine.h"
#include <QString>
#include <QLatin1String>
#include <QTemporaryFile>

#include <QDebug>

using namespace Mdt::ExecutableFile;

TEST_CASE("supportsPlatform")
{
  PeFileIoEngine engine;

  SECTION("PE")
  {
    const Platform platform(OperatingSystem::Windows, ExecutableFileFormat::Pe, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( engine.supportsPlatform(platform) );
  }

  SECTION("ELF")
  {
    const Platform platform(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( !engine.supportsPlatform(platform) );
  }
}

TEST_CASE("open_close")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  PeFileIoEngine engine;
  REQUIRE( !engine.isOpen() );

  SECTION("empty file")
  {
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isOpen() );
    engine.close();
    REQUIRE( !engine.isOpen() );
  }
}

TEST_CASE("isPeImageFile")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  PeFileIoEngine engine;

  SECTION("empty file")
  {
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isPeImageFile() );
  }

  SECTION("text file - 3 chars")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABC") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isPeImageFile() );
  }

  SECTION("text file - 60 chars (0x3C)")
  {
    REQUIRE( writeTextFileUtf8( file, generateStringWithNChars(0x3c) ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isPeImageFile() );
  }

  SECTION("text file")
  {
    REQUIRE( writeTextFileUtf8( file, generateStringWithNChars(300) ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isPeImageFile() );
  }
}

TEST_CASE("isExecutableOrSharedLibrary")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  PeFileIoEngine engine;

  SECTION("empty file")
  {
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }

  SECTION("text file - 3 chars")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABC") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }

  SECTION("text file - 4 chars")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABCD") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }

  SECTION("text file - 64 chars")
  {
    REQUIRE( writeTextFileUtf8( file, generateStringWithNChars(64) ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }
}

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
#include "TestFileUtils.h"
#include "Mdt/ExecutableFile/ElfFileIoEngine.h"
#include <QString>
#include <QLatin1String>
#include <QTemporaryFile>
#include <cassert>

using namespace Mdt::ExecutableFile;

TEST_CASE("supportsPlatform")
{
  ElfFileIoEngine engine;

  SECTION("ELF")
  {
    const Platform platform(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( engine.supportsPlatform(platform) );
  }

  SECTION("PE")
  {
    const Platform platform(OperatingSystem::Windows, ExecutableFileFormat::Pe, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( !engine.supportsPlatform(platform) );
  }
}

TEST_CASE("open_close")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  ElfFileIoEngine engine;
  REQUIRE( !engine.isOpen() );

  SECTION("empty file")
  {
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( engine.isOpen() );
    engine.close();
    REQUIRE( !engine.isOpen() );
  }
}

TEST_CASE("isElfFile")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  ElfFileIoEngine engine;

  SECTION("empty file")
  {
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isElfFile() );
  }

  SECTION("text file - 3 chars")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABC") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isElfFile() );
  }

  SECTION("text file - 4 chars")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABCD") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isElfFile() );
  }

  SECTION("text file")
  {
    REQUIRE( writeTextFileUtf8( file, QLatin1String("ABCDEFGHIJKLMNOPQRSTUWXYZ") ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isElfFile() );
  }
}

TEST_CASE("isExecutableOrSharedLibrary")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  ElfFileIoEngine engine;

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
    const QString text = QLatin1String(
      "0123456789"
      "0123456789"
      "0123456789"
      "0123456789"
      "0123456789"
      "0123456789"
      "1234"
    );
    REQUIRE( writeTextFileUtf8( file, text ) );
    file.close();
    engine.openFile( file.fileName(), ExecutableFileOpenMode::ReadOnly );
    REQUIRE( !engine.isExecutableOrSharedLibrary() );
    engine.close();
  }
}

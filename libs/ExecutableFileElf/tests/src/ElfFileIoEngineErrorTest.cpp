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

#include "Mdt/ExecutableFile/ElfFileIoEngine.h"
#include "Mdt/ExecutableFile/NotNullTerminatedStringError.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include <QTemporaryDir>
#include <QDir>
#include <QTemporaryFile>
#include <QString>
#include <QLatin1String>
#include <cassert>


using namespace Mdt::ExecutableFile;

QString makeFilePath(const QTemporaryDir & dir, const QString & fileName)
{
  assert( dir.isValid() );

  return QDir::cleanPath( dir.path() + QLatin1Char('/') + fileName );
}

TEST_CASE("stringFromUnsignedCharArray")
{
  using Elf::stringFromUnsignedCharArray;

  ByteArraySpan span;

  SECTION("A (no end of string)")
  {
    unsigned char array[1] = {'A'};
    span.data = array;
    span.size = 1;
    REQUIRE_THROWS_AS( stringFromUnsignedCharArray(span), NotNullTerminatedStringError );
  }
}

TEST_CASE("open_close")
{
  QString filePath;

  QTemporaryDir dir;
  REQUIRE( dir.isValid() );

  ElfFileIoEngine engine;
  REQUIRE( !engine.isOpen() );

  SECTION("non existing file")
  {
    filePath = makeFilePath( dir, QLatin1String("nonExisting.so") );
    REQUIRE_THROWS_AS( engine.openFile(filePath, ExecutableFileOpenMode::ReadOnly), FileOpenError );
    REQUIRE( !engine.isOpen() );
  }
}

/*
 * See preconditions on ElfFileIoEngine::getNeededSharedLibraries()
 *
TEST_CASE("getNeededSharedLibraries")
{
  QTemporaryFile file;
  REQUIRE( file.open() );

  ElfFileIoEngine engine;

  SECTION("empty file")
  {
    file.close();
    engine.openFile( file.fileName() );
    REQUIRE_THROWS_AS( engine.getNeededSharedLibraries(), ExecutableFileReadError );
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
    engine.openFile( file.fileName() );
    REQUIRE_THROWS_AS( engine.getNeededSharedLibraries(), ExecutableFileReadError );
    engine.close();
  }

  SECTION("other cases")
  {
    REQUIRE(false);
  }
}
*/

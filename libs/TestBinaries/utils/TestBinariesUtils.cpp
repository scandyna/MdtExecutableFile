// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "TestBinariesUtils.h"
#include <QFileInfo>
#include <cassert>


QString qt5CoreFilePath()
{
  auto path = QString::fromLocal8Bit(QT5_CORE_FILE_PATH);
  assert( QFileInfo(path).isAbsolute() );

  return path;
}

QString qt5CoreFileName()
{
  return QFileInfo( qt5CoreFilePath() ).fileName();
}

bool containsQt5Core(const QStringList & libraries)
{
  return libraries.contains( qt5CoreFileName() );
}


QString testSharedLibraryFilePath()
{
  auto path = QString::fromLocal8Bit(TEST_SHARED_LIBRARY_FILE_PATH);
  assert( QFileInfo(path).isAbsolute() );

  return path;
}

QString testSharedLibraryFileName()
{
  return QFileInfo( testSharedLibraryFilePath() ).fileName();
}

bool containsTestSharedLibrary(const QStringList & libraries)
{
  return libraries.contains( testSharedLibraryFileName() );
}

QString testExecutableFilePath()
{
  auto path = QString::fromLocal8Bit(TEST_DYNAMIC_EXECUTABLE_FILE_PATH);
  assert( QFileInfo(path).isAbsolute() );

  return path;
}


QString testStaticLibraryFilePath()
{
  auto path = QString::fromLocal8Bit(TEST_STATIC_LIBRARY_FILE_PATH);
  assert( QFileInfo(path).isAbsolute() );

  return path;
}

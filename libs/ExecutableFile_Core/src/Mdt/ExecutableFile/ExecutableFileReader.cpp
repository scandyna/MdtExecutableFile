// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ExecutableFileReader.h"
#include "AbstractExecutableFileIoEngine.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

ExecutableFileReader::ExecutableFileReader(QObject* parent)
 : QObject(parent)
{
}

void ExecutableFileReader::openFile(const QFileInfo & fileInfo)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !isOpen() );

  mEngine.openFile(fileInfo, ExecutableFileOpenMode::ReadOnly);
}

void ExecutableFileReader::openFile(const QFileInfo & fileInfo, const Platform & platform)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !platform.isNull() );
  assert( !isOpen() );

  mEngine.openFile(fileInfo, ExecutableFileOpenMode::ReadOnly, platform);
}

bool ExecutableFileReader::isOpen() const noexcept
{
  return mEngine.isOpen();
}

void ExecutableFileReader::close()
{
  mEngine.close();
}

Platform ExecutableFileReader::getFilePlatform()
{
  assert( isOpen() );

  return mEngine.engine()->getFilePlatform();
}

bool ExecutableFileReader::isExecutableOrSharedLibrary()
{
  assert( isOpen() );

  return mEngine.engine()->isExecutableOrSharedLibrary();
}

bool ExecutableFileReader::containsDebugSymbols()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return mEngine.engine()->containsDebugSymbols();
}

QStringList ExecutableFileReader::getNeededSharedLibraries()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return mEngine.engine()->getNeededSharedLibraries();
}

RPath ExecutableFileReader::getRunPath()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return mEngine.engine()->getRunPath();
}

}} // namespace Mdt{ namespace ExecutableFile{

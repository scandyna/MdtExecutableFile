// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ExecutableFileWriter.h"
#include "Mdt/ExecutableFile/ExecutableFileIoEngineImplementationInterface.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

ExecutableFileWriter::ExecutableFileWriter(QObject *parent)
 : QObject(parent)
{
  connect(&mEngine, &ExecutableFileIoEngine::message, this, &ExecutableFileWriter::message);
  connect(&mEngine, &ExecutableFileIoEngine::verboseMessage, this, &ExecutableFileWriter::verboseMessage);
}

void ExecutableFileWriter::openFile(const QFileInfo & fileInfo)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !isOpen() );

  mEngine.openFile(fileInfo, ExecutableFileOpenMode::ReadWrite);
}

void ExecutableFileWriter::openFile(const QFileInfo & fileInfo, const Platform & platform)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !platform.isNull() );
  assert( !isOpen() );

  mEngine.openFile(fileInfo, ExecutableFileOpenMode::ReadWrite, platform);
}

bool ExecutableFileWriter::isOpen() const noexcept
{
  return mEngine.isOpen();
}

void ExecutableFileWriter::close()
{
  mEngine.close();
}

bool ExecutableFileWriter::isExecutableOrSharedLibrary()
{
  assert( isOpen() );

  return mEngine.engine()->isExecutableOrSharedLibrary();
}

RPath ExecutableFileWriter::getRunPath()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return mEngine.engine()->getRunPath();
}

void ExecutableFileWriter::setRunPath(const RPath & rPath)
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  mEngine.engine()->setRunPath(rPath);
}

}} // namespace Mdt{ namespace ExecutableFile{

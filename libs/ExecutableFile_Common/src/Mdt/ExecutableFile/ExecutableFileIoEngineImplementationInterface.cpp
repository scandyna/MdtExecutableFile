// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ExecutableFileIoEngineImplementationInterface.h"

namespace Mdt{ namespace ExecutableFile{

ExecutableFileIoEngineImplementationInterface::ExecutableFileIoEngineImplementationInterface(QObject* parent)
 : QObject(parent)
{
}

void ExecutableFileIoEngineImplementationInterface::openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !isOpen() );

  if( !fileInfo.exists() ){
    const QString message = tr("file '%1' does not exist")
                            .arg( fileInfo.absoluteFilePath() );
    throw FileOpenError(message);
  }

  mFile.setFileName( fileInfo.absoluteFilePath() );
  const auto openMode = qIoDeviceOpenModeFromOpenMode(mode);
  if( !mFile.open(openMode) ){
    const QString message = tr("could not open file '%1': %2")
                            .arg( fileInfo.absoluteFilePath(), mFile.errorString() );
    throw FileOpenError(message);
  }

  newFileOpen( mFile.fileName() );
}

void ExecutableFileIoEngineImplementationInterface::close()
{
  mFileMapper.unmap(mFile);
  mFile.close();
  fileClosed();
}

bool ExecutableFileIoEngineImplementationInterface::isElfFile()
{
  assert( isOpen() );

  return doIsElfFile();
}

bool ExecutableFileIoEngineImplementationInterface::isPeImageFile()
{
  assert( isOpen() );

  return doIsPeImageFile();
}

Platform ExecutableFileIoEngineImplementationInterface::getFilePlatform()
{
  assert( isOpen() );

  return doGetFilePlatform();
}

bool ExecutableFileIoEngineImplementationInterface::isExecutableOrSharedLibrary()
{
  assert( isOpen() );

  return doIsExecutableOrSharedLibrary();
}

bool ExecutableFileIoEngineImplementationInterface::containsDebugSymbols()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doContainsDebugSymbols();
}

QStringList ExecutableFileIoEngineImplementationInterface::getNeededSharedLibraries()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doGetNeededSharedLibraries();
}

RPath ExecutableFileIoEngineImplementationInterface::getRunPath()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doGetRunPath();
}

void ExecutableFileIoEngineImplementationInterface::setRunPath(const RPath & rPath)
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  doSetRunPath(rPath);
}

qint64 ExecutableFileIoEngineImplementationInterface::fileSize() const noexcept
{
  assert( isOpen() );

  return mFile.size();
}

void ExecutableFileIoEngineImplementationInterface::resizeFile(qint64 size)
{
  assert( isOpen() );
  assert( size > 0 );

  if( !mFile.resize(size) ){
    const QString msg = tr("resize file '%1' failed: %2")
                        .arg( fileName(), mFile.errorString() );
    throw ExecutableFileWriteError(msg);
  }
}



QString ExecutableFileIoEngineImplementationInterface::fileName() const noexcept
{
  assert( isOpen() );

  return mFile.fileName();
}

ByteArraySpan ExecutableFileIoEngineImplementationInterface::mapIfRequired(qint64 offset, qint64 size)
{
  assert( isOpen() );

  return mFileMapper.mapIfRequired(mFile, offset, size);
}

void ExecutableFileIoEngineImplementationInterface::doSetRunPath(const RPath &)
{
}

QIODevice::OpenMode ExecutableFileIoEngineImplementationInterface::qIoDeviceOpenModeFromOpenMode(ExecutableFileOpenMode mode) noexcept
{
  switch(mode){
    case ExecutableFileOpenMode::ReadOnly:
      return QIODevice::ReadOnly;
    case ExecutableFileOpenMode::ReadWrite:
      return QIODevice::ReadWrite;
  }

  return QIODevice::ReadOnly;
}

}} // namespace Mdt{ namespace ExecutableFile{

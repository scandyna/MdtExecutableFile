// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "AbstractExecutableFileIoEngine.h"

namespace Mdt{ namespace ExecutableFile{

AbstractExecutableFileIoEngine::AbstractExecutableFileIoEngine(QObject* parent)
 : QObject(parent)
{
}

void AbstractExecutableFileIoEngine::openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode)
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

void AbstractExecutableFileIoEngine::close()
{
  mFileMapper.unmap(mFile);
  mFile.close();
  fileClosed();
}

bool AbstractExecutableFileIoEngine::isElfFile()
{
  assert( isOpen() );

  return doIsElfFile();
}

bool AbstractExecutableFileIoEngine::isPeImageFile()
{
  assert( isOpen() );

  return doIsPeImageFile();
}

Platform AbstractExecutableFileIoEngine::getFilePlatform()
{
  assert( isOpen() );

  return doGetFilePlatform();
}

bool AbstractExecutableFileIoEngine::isExecutableOrSharedLibrary()
{
  assert( isOpen() );

  return doIsExecutableOrSharedLibrary();
}

bool AbstractExecutableFileIoEngine::containsDebugSymbols()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doContainsDebugSymbols();
}

QStringList AbstractExecutableFileIoEngine::getNeededSharedLibraries()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doGetNeededSharedLibraries();
}

RPath AbstractExecutableFileIoEngine::getRunPath()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  return doGetRunPath();
}

void AbstractExecutableFileIoEngine::setRunPath(const RPath & rPath)
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  doSetRunPath(rPath);
}

qint64 AbstractExecutableFileIoEngine::fileSize() const noexcept
{
  assert( isOpen() );

  return mFile.size();
}

void AbstractExecutableFileIoEngine::resizeFile(qint64 size)
{
  assert( isOpen() );
  assert( size > 0 );

  if( !mFile.resize(size) ){
    const QString msg = tr("resize file '%1' failed: %2")
                        .arg( fileName(), mFile.errorString() );
    throw ExecutableFileWriteError(msg);
  }
}



QString AbstractExecutableFileIoEngine::fileName() const noexcept
{
  assert( isOpen() );

  return mFile.fileName();
}

ByteArraySpan AbstractExecutableFileIoEngine::mapIfRequired(qint64 offset, qint64 size)
{
  assert( isOpen() );

  return mFileMapper.mapIfRequired(mFile, offset, size);
}

void AbstractExecutableFileIoEngine::doSetRunPath(const RPath &)
{
}

QIODevice::OpenMode AbstractExecutableFileIoEngine::qIoDeviceOpenModeFromOpenMode(ExecutableFileOpenMode mode) noexcept
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

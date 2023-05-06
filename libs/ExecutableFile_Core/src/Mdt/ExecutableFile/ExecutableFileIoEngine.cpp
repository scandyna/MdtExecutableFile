// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ExecutableFileIoEngine.h"
#include "Mdt/ExecutableFile/AbstractExecutableFileIoEngine.h"
#include "Mdt/ExecutableFile/ExecutableFileFormat.h"
#include "Mdt/ExecutableFile/ElfFileIoEngine.h"
#include "Mdt/ExecutableFile/PeFileIoEngine.h"
#include <cassert>


namespace Mdt{ namespace ExecutableFile{

ExecutableFileIoEngine::ExecutableFileIoEngine(QObject *parent)
 : QObject(parent)
{
}

ExecutableFileIoEngine::~ExecutableFileIoEngine() noexcept
{
}

void ExecutableFileIoEngine::openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !isOpen() );

  const auto hostPlatform = Platform::nativePlatform();

  if( !mIoEngine ){
    instanciateEngine( hostPlatform.executableFileFormat() );
  }
  assert( mIoEngine.get() != nullptr );

  mIoEngine->openFile(fileInfo, mode);

  if( hostPlatform.operatingSystem() == OperatingSystem::Linux ){
    if( !mIoEngine->isElfFile() ){
      mIoEngine->close();
      mIoEngine.reset();
      instanciateEngine(ExecutableFileFormat::Pe);
      mIoEngine->openFile(fileInfo, mode);
    }
  }

  if( hostPlatform.operatingSystem() == OperatingSystem::Windows ){
    if( !mIoEngine->isPeImageFile() ){
      mIoEngine->close();
      mIoEngine.reset();
      instanciateEngine(ExecutableFileFormat::Elf);
      mIoEngine->openFile(fileInfo, mode);
    }
  }
}

void ExecutableFileIoEngine::openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode, const Platform & platform)
{
  assert( !fileInfo.filePath().isEmpty() );
  assert( !platform.isNull() );
  assert( !isOpen() );

  if( !mIoEngine ){
    instanciateEngine( platform.executableFileFormat() );
  }
  assert( mIoEngine.get() != nullptr );

  if( !mIoEngine->supportsPlatform(platform) ){
    mIoEngine.reset();
    instanciateEngine( platform.executableFileFormat() );
  }
  assert( mIoEngine.get() != nullptr );

  mIoEngine->openFile(fileInfo, mode);

  Platform filePlatform;
  try{
    filePlatform = getFilePlatform();
  }catch(const ExecutableFileReadError &){
  }

  if( filePlatform != platform ){
    const QString message = tr("File '%1' is not of the requested platform")
                            .arg( fileInfo.absoluteFilePath() );
    throw FileOpenError(message);
  }
}

bool ExecutableFileIoEngine::isOpen() const noexcept
{
  if( !mIoEngine ){
    return false;
  }
  return mIoEngine->isOpen();
}

void ExecutableFileIoEngine::close()
{
  if(mIoEngine){
    mIoEngine->close();
  }
}

Platform ExecutableFileIoEngine::getFilePlatform()
{
  assert( isOpen() );
  assert( mIoEngine.get() != nullptr );

  return mIoEngine->getFilePlatform();
}

void ExecutableFileIoEngine::instanciateEngine(ExecutableFileFormat format) noexcept
{
  assert( mIoEngine.get() == nullptr );
  assert( format != ExecutableFileFormat::Unknown );

  switch(format){
    case ExecutableFileFormat::Elf:
      mIoEngine = std::make_unique<ElfFileIoEngine>();
      break;
    case ExecutableFileFormat::Pe:
      mIoEngine = std::make_unique<PeFileIoEngine>();
      break;
    case ExecutableFileFormat::Unknown:
      break;
  }

  if( mIoEngine.get() != nullptr ){
    connect(mIoEngine.get(), &AbstractExecutableFileIoEngine::message, this, &ExecutableFileIoEngine::message);
    connect(mIoEngine.get(), &AbstractExecutableFileIoEngine::verboseMessage, this, &ExecutableFileIoEngine::verboseMessage);
  }
}

}} // namespace Mdt{ namespace ExecutableFile{

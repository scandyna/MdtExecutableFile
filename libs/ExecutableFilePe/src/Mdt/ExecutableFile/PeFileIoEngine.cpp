// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "PeFileIoEngine.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "Mdt/ExecutableFile/Pe/FileReader.h"
#include <cstdint>

namespace Mdt{ namespace ExecutableFile{

PeFileIoEngine::PeFileIoEngine(QObject *parent)
  : AbstractExecutableFileIoEngine(parent),
    mImpl( std::make_unique<Pe::FileReader>() )
{
}

PeFileIoEngine::~PeFileIoEngine() noexcept
{
}

void PeFileIoEngine::newFileOpen(const QString & fileName)
{
  mImpl->setFileName(fileName);
}

void PeFileIoEngine::fileClosed()
{
  mImpl->clear();
}

bool PeFileIoEngine::doSupportsPlatform(const Platform & platform) const noexcept
{
  return platform.executableFileFormat() == ExecutableFileFormat::Pe;
}

bool PeFileIoEngine::doIsPeImageFile()
{
  return tryExtractDosCoffAndOptionalHeader();
}

Platform PeFileIoEngine::doGetFilePlatform()
{
  using Pe::CoffHeader;
  using Pe::MachineType;

  const auto os = OperatingSystem::Windows;
  const auto fileFormat = ExecutableFileFormat::Pe;
  const auto fakeCompiler = Compiler::Gcc;

  if( !tryExtractDosCoffAndOptionalHeader() ){
    const QString message = tr("file '%1' is not a valid PE image")
                            .arg( fileName() );
    throw ExecutableFileReadError(message);
  }

  ProcessorISA cpu;
  switch( mImpl->coffHeader().machineType() ){
    case MachineType::I386:
      cpu = ProcessorISA::X86_32;
      break;
    case MachineType::Amd64:
      cpu = ProcessorISA::X86_64;
      break;
    default:
      cpu = ProcessorISA::Unknown;
  }

  return Platform(os, fileFormat, fakeCompiler, cpu);
}

bool PeFileIoEngine::doIsExecutableOrSharedLibrary()
{
  if( !tryExtractDosCoffAndOptionalHeader() ){
    return false;
  }
  if( !mImpl->isValidExecutableImage() ){
    return false;
  }

  return true;
}

bool PeFileIoEngine::doContainsDebugSymbols()
{
  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl->containsDebugSymbols(map);
}

QStringList PeFileIoEngine::doGetNeededSharedLibraries()
{
  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl->getNeededSharedLibraries(map);
}

bool PeFileIoEngine::tryExtractDosCoffAndOptionalHeader()
{
  int64_t size = 64;
  if( fileSize() < size ){
    return false;
  }

  ByteArraySpan map = mapIfRequired(0, size);
  if( !mImpl->tryExtractDosHeader(map) ){
    return false;
  }

  size = mImpl->minimumSizeToExtractCoffHeader();
  if( fileSize() < size ){
    return false;
  }

  map = mapIfRequired(0, size);
  if( !mImpl->tryExtractCoffHeader(map) ){
    return false;
  }

  size = mImpl->minimumSizeToExtractOptionalHeader();
  if( fileSize() < size ){
    return false;
  }

  map = mapIfRequired(0, size);
  if( !mImpl->tryExtractOptionalHeader(map) ){
    return false;
  }

  return true;
}

}} // namespace Mdt{ namespace ExecutableFile{

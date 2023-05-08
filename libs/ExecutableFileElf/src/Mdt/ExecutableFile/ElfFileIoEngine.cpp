// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ElfFileIoEngine.h"
#include "Mdt/ExecutableFile/RPathElf.h"
#include "Mdt/ExecutableFile/Elf/FileWriterFile.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

ElfFileIoEngine::ElfFileIoEngine(QObject *parent)
  : ExecutableFileIoEngineImplementationInterface(parent)
{
}

QString ElfFileIoEngine::getSoName()
{
  assert( isOpen() );
  assert( isExecutableOrSharedLibrary() );

  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl.getSoName(map);
}

void ElfFileIoEngine::newFileOpen(const QString & fileName)
{
  mImpl.setFileName(fileName);
}

void ElfFileIoEngine::fileClosed()
{
  mImpl.clear();
}

bool ElfFileIoEngine::doSupportsPlatform(const Platform & platform) const noexcept
{
  return platform.executableFileFormat() == ExecutableFileFormat::Elf;
}

bool ElfFileIoEngine::doIsElfFile()
{
  using Elf::Ident;
  using Elf::extractIdent;

  const qint64 size = 16;

  if( fileSize() < size ){
    return false;
  }

  const ByteArraySpan map = mapIfRequired(0, size);

  const Ident ident = extractIdent(map);

  return ident.isValid();
}

Platform ElfFileIoEngine::doGetFilePlatform()
{
  using Elf::FileHeader;
  using Elf::OsAbiType;
  using Elf::Machine;

  const int64_t size = mImpl.minimumSizeToReadFileHeader();

  if( fileSize() < size ){
    const QString message = tr("file '%1' is to small to read the file header")
                            .arg( fileName() );
    throw ExecutableFileReadError(message);
  }

  const ByteArraySpan map = mapIfRequired(0, size);

  const FileHeader fileHeader = mImpl.getFileHeader(map);
  assert( fileHeader.seemsValid() );

  const auto fileFormat = ExecutableFileFormat::Elf;

  OperatingSystem os;
  switch( fileHeader.ident.osAbiType() ){
    case OsAbiType::SystemV:
    case OsAbiType::Linux:
      os = OperatingSystem::Linux;
      break;
    default:
      os = OperatingSystem::Unknown;
  }

  ProcessorISA cpu;
  switch( fileHeader.machineType() ){
    case Machine::X86:
      cpu = ProcessorISA::X86_32;
      break;
    case Machine::X86_64:
      cpu = ProcessorISA::X86_64;
      break;
    default:
      cpu = ProcessorISA::Unknown;
  }

  const auto fakeCompiler = Compiler::Gcc;

  return Platform(os, fileFormat, fakeCompiler, cpu);
}

bool ElfFileIoEngine::doIsExecutableOrSharedLibrary()
{
  using Elf::FileHeader;
  using Elf::ObjectFileType;
  using Elf::extractFileHeader;

  const int64_t size = mImpl.minimumSizeToReadFileHeader();

  if( fileSize() < size ){
    return false;
  }

  const ByteArraySpan map = mapIfRequired(0, size);

  const FileHeader fileHeader = extractFileHeader(map);

  if( !fileHeader.seemsValid() ){
    return false;
  }

  if(fileHeader.objectFileType() == ObjectFileType::ExecutableFile){
    return true;
  }
  if(fileHeader.objectFileType() == ObjectFileType::SharedObject){
    return true;
  }

  return false;
}

bool ElfFileIoEngine::doContainsDebugSymbols()
{
  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl.containsDebugSymbols(map);
}

QStringList ElfFileIoEngine::doGetNeededSharedLibraries()
{
  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl.getNeededSharedLibraries(map);
}

RPath ElfFileIoEngine::doGetRunPath()
{
  const qint64 size = fileSize();

  const ByteArraySpan map = mapIfRequired(0, size);

  return mImpl.getRunPath(map);
}

void ElfFileIoEngine::doSetRunPath(const RPath & rPath)
{
  using Elf::FileWriterFile;

  const qint64 size = fileSize();

  ByteArraySpan map = mapIfRequired(0, size);

  FileWriterFile file;
  connect(&file, &FileWriterFile::message, this, &ElfFileIoEngine::message);
  connect(&file, &FileWriterFile::verboseMessage, this, &ElfFileIoEngine::verboseMessage);

  mImpl.readToFileWriterFile(file, map);

  file.setRunPath( RPathElf::rPathToString(rPath) );

  const qint64 newSize = file.minimumSizeToWriteFile();
  if(newSize > size){
    resizeFile(newSize);
    map = mapIfRequired(0, newSize);
  }

  mImpl.setFileWriterToMap(map, file);
}

}} // namespace Mdt{ namespace ExecutableFile{

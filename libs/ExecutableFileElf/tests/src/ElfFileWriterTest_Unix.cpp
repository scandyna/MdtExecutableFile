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
#include "TestUtils.h"
#include "TestFileUtils.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeadersReader.h"
#include "Mdt/ExecutableFile/Elf/SymbolTableReader.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableReader.h"
#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSectionReader.h"
#include "Mdt/ExecutableFile/Elf/GnuHashTableReader.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionReader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterFile.h"
#include "Mdt/ExecutableFile/Elf/FileWriter.h"
#include "Mdt/ExecutableFile/QRuntimeError.h"
#include <QTemporaryDir>
#include <QFile>
#include <QString>
#include <QLatin1String>
#include <QDebug>
#include <vector>
#include <cassert>

// #include "Mdt/ExecutableFile/Elf/Debug.h"
// #include <iostream>

using namespace Mdt::ExecutableFile;
using Elf::FileHeader;
using Elf::FileAllHeaders;
using Elf::DynamicSection;
using Elf::FileWriterFile;

void openFile(QFile & file, const QString & executableFilePath, QIODevice::OpenMode mode)
{
  assert( !file.isOpen() );

  file.setFileName(executableFilePath);
  if( !file.open(mode) ){
    const QString msg = QLatin1String("open file '") + executableFilePath + QLatin1String(" failed: ") + file.errorString();
    throw QRuntimeError(msg);
  }
}

void openFileForWrite(QFile & file, const QString & executableFilePath)
{
  assert( !file.isOpen() );

  openFile(file, executableFilePath, QIODevice::ReadWrite);
}

void resizeFile(QFile & file, qint64 size)
{
  assert( file.isOpen() );
  assert( size > 0 );

  if( !file.resize(size) ){
    const QString msg = QLatin1String("resize file '") + file.fileName() + QLatin1String(" failed: ") + file.errorString();
    throw QRuntimeError(msg);
  }
}

ByteArraySpan mapFile(QFile & file)
{
  ByteArraySpan map;

  const qint64 size = file.size();
  map.data = file.map(0, size);
  map.size = size;

  if( map.isNull() ){
    const QString msg = QLatin1String("map file '") + file.fileName() + QLatin1String(" failed: ") + file.errorString();
    throw QRuntimeError(msg);
  }

  return map;
}

ByteArraySpan openAndMapFile(QFile & file, const QString & executableFilePath, QIODevice::OpenMode mode)
{
  assert( !file.isOpen() );

  openFile(file, executableFilePath, mode);

  return mapFile(file);
}

ByteArraySpan openAndMapFileForRead(QFile & file, const QString & executableFilePath)
{
  assert( !file.isOpen() );

  return openAndMapFile(file, executableFilePath, QIODevice::ReadOnly);
}

ByteArraySpan openAndMapFileForWrite(QFile & file, const QString & executableFilePath)
{
  assert( !file.isOpen() );

  return openAndMapFile(file, executableFilePath, QIODevice::ReadWrite);
}

bool unmapAndCloseFile(QFile & file, ByteArraySpan & map)
{
  assert( file.isOpen() );
  assert( !map.isNull() );

  const QString filePath = file.fileName();

  if( !file.unmap(map.data) ){
    const QString msg = QLatin1String("unmap file '") + filePath + QLatin1String(" failed: ") + file.errorString();
    return false;
  }
  map.data = nullptr;
  map.size = 0;
  if( !file.flush() ){
    const QString msg = QLatin1String("flush file '") + filePath + QLatin1String(" failed: ") + file.errorString();
    return false;
  }
  file.close();

  return true;
}

void readElfFile(FileWriterFile & elfFile, const QString & filePath)
{
  using Elf::extractFileHeader;
  using Elf::extractAllHeaders;
  using Elf::extractDynamicSection;
  using Elf::extractSymTabPartReferringToSection;
  using Elf::extractDynSymPartReferringToSection;
  using Elf::extractGotSection;
  using Elf::extractGotPltSection;
  using Elf::extractProgramInterpreterSection;
  using Elf::GnuHashTableReader;
  using Elf::GnuHashTableReadError;
  using Elf::NoteSectionReader;
  using Elf::NoteSectionReadError;

//   FileWriterFile elfFile;
  QFile file(filePath);
  ByteArraySpan map;

  map = openAndMapFileForRead(file, filePath);

  qDebug() << "readElfFile() - extracting file header ...";
  const FileHeader fileHeader = extractFileHeader(map);
  if( !fileHeader.seemsValid() ){
    const QString msg = QLatin1String("file does not contain a valid file header");
    throw QRuntimeError(msg);
  }

  qDebug() << "readElfFile() - extracting all headers ...";
  const FileAllHeaders headers = extractAllHeaders(map, fileHeader);
  if( !headers.seemsValid() ){
    const QString msg = QLatin1String("file contains some invalid header");
    throw QRuntimeError(msg);
  }

//   elfFile = FileWriterFile::fromOriginalFile( headers, extractDynamicSection( map, fileHeader, headers.sectionNameStringTableHeader() ) );
  elfFile.setHeadersFromFile(headers);
  elfFile.setDynamicSectionFromFile( extractDynamicSection( map, fileHeader, headers.sectionNameStringTableHeader() ) );
  elfFile.setSymTabFromFile( extractSymTabPartReferringToSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
  elfFile.setDynSymFromFile( extractDynSymPartReferringToSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );

  if( headers.containsGotSectionHeader() ){
    elfFile.setGotSectionFromFile( extractGotSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
  }

  ///elfFile.setGotPltSectionFromFile( extractGotPltSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );

  if( headers.containsProgramInterpreterSectionHeader() ){
    elfFile.setProgramInterpreterSectionFromFile( extractProgramInterpreterSection( map, headers.programInterpreterSectionHeader() ) );
  }

  if( headers.containsGnuHashTableSectionHeader() ){
    elfFile.setGnuHashTableSection( GnuHashTableReader::extractHasTable( map, headers.fileHeader(), headers.gnuHashTableSectionHeader() ) );
  }

  try{
    elfFile.setNoteSectionTableFromFile( NoteSectionReader::extractNoteSectionTable( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
  }catch(const NoteSectionReadError & error){
    const QString msg = QString::fromLatin1("file contains a invalid note section: %1").arg( error.whatQString() );
    throw QRuntimeError(msg);
  }

  if( !unmapAndCloseFile(file, map) ){
    const QString msg = QLatin1String("unmap/close file failed");
    throw QRuntimeError(msg);
  }

//   return elfFile;
}

void copyAndReadElfFile(FileWriterFile & elfFile, const QString & targetFilePath, const char * sourceFile)
{
  const QString sourceFilePath = QString::fromLocal8Bit(sourceFile);
  qDebug() << "using source file: " << sourceFilePath;

  if( !copyFile(sourceFilePath, targetFilePath) ){
    const QString msg = QLatin1String("copy the file failed");
    throw QRuntimeError(msg);
  }

//   if( !runExecutable(targetFilePath) ){
//     const QString msg = QLatin1String("executing copied file failed");
//     throw QRuntimeError(msg);
//   }

  readElfFile(elfFile, targetFilePath);
}

bool readExecutable(const QString & filePath)
{
//   using Impl::Elf::toDebugString;

  FileWriterFile elfFile;
  readElfFile(elfFile, filePath);
  if( !elfFile.seemsValid() ){
    return false;
  }

//   std::cout << "File header:\n" << toDebugString(elfFile.fileHeader()).toStdString() << std::endl;
//   std::cout << "Program headers:\n" << toDebugString(elfFile.programHeaderTable()).toStdString() << std::endl;
//   std::cout << "Section headers:\n" << toDebugString(elfFile.sectionHeaderTable()).toStdString() << std::endl;
//   std::cout << "Program interpreter:\n" << toDebugString(elfFile.programInterpreterSection()).toStdString() << std::endl;
//   std::cout << "Note sections:" << toDebugString(elfFile.noteSectionTable()) << std::endl;
//   std::cout << "Dynamic section:\n" << toDebugString( elfFile.dynamicSection() ).toStdString() << std::endl;
//   std::cout << "Dynamic section string table:\n" << toDebugString( elfFile.dynamicSection().stringTable() ).toStdString() << std::endl;
//   std::cout << "partial symbol table .symtab:" << toDebugString( elfFile.symTab() ).toStdString() << std::endl;
//   std::cout << "partial symbol table .dynsym:" << toDebugString( elfFile.dynSym() ).toStdString() << std::endl;
//   std::cout << "global offset table .got:" << toDebugString( elfFile.gotSection() ).toStdString() << std::endl;
//   std::cout << "global offset table .got.plt:" << toDebugString( elfFile.gotPltSection() ).toStdString() << std::endl;
//   std::cout << toDebugString( elfFile.gnuHashTableSection() ).toStdString() << std::endl;
//   std::cout << "File layout:\n" << fileLayoutToDebugString( elfFile.fileHeader(), elfFile.programHeaderTable(), elfFile.sectionHeaderTable() ).toStdString() << std::endl;
//   std::cout << sectionSegmentMappingToDebugString( elfFile.headers() ).toStdString() << std::endl;
//   std::cout << "RunPath: " << elfFile.dynamicSection().getRunPath().toStdString() << std::endl;

  return true;
}

bool runElfExecutable(const QString & executableFilePath)
{
  if( !setFileExePermissionsIfRequired(executableFilePath) ){
    return false;
  }

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert( QLatin1String("LD_BIND_NOW"), QLatin1String("1") );
//   env.insert( QLatin1String("LD_DEBUG"), QLatin1String("all") );

  return runExecutable(executableFilePath, QStringList(), env);
}

bool lintElfFile(const QString & filePath)
{
  const QStringList arguments = {filePath,QLatin1String("--strict"),QLatin1String("--gnu-ld")};

  return runExecutable(QLatin1String("eu-elflint"), arguments);
}

QString getExecutableRunPath(const QString & filePath)
{
  FileWriterFile elfFile;
  readElfFile(elfFile, filePath);
  if( !elfFile.seemsValid() ){
    return QString();
  }

  return elfFile.dynamicSection().getRunPath();
}

/*
 * Here we simply read a ELF executable
 * then write it back, without changing anything.
 *
 * The purpose is to check if writing the original content works
 */
TEST_CASE("simpleReadWrite")
{
  using Elf::setFileToMap;

  QTemporaryDir dir;
  REQUIRE( dir.isValid() );
  dir.setAutoRemove(true);

  FileWriterFile elfFile;
  QFile file;
  ByteArraySpan map;

  const QString sourceFilePath = QString::fromLocal8Bit(TEST_SIMPLE_EXECUTABLE_DYNAMIC_FILE_PATH);
  qDebug() << "using source file: " << sourceFilePath;

  const QString targetFilePath = makePath(dir, "targetFile");
  REQUIRE( copyFile(sourceFilePath, targetFilePath) );
  REQUIRE( runElfExecutable(targetFilePath) );

  readElfFile(elfFile, targetFilePath);
  REQUIRE( elfFile.seemsValid() );

  map = openAndMapFileForWrite(file, targetFilePath);
  REQUIRE( !map.isNull() );

  setFileToMap(map, elfFile);

  unmapAndCloseFile(file, map);
  REQUIRE( runElfExecutable(targetFilePath) );

  /// \todo create a function that takes expected headers and reads the file back
  /*
   * Read the file again
   * (Sections are not used for execution)
   */
  REQUIRE( readExecutable(targetFilePath) );
  REQUIRE( lintElfFile(targetFilePath) );
}

TEST_CASE("editRunPath_SimpleExecutable")
{
  using Elf::setFileToMap;

  QTemporaryDir dir;
  REQUIRE( dir.isValid() );
  dir.setAutoRemove(true);

  FileWriterFile elfFile;
  QFile file;
  ByteArraySpan map;

  SECTION("dynamic section does not change")
  {
    const QString targetFilePath = makePath(dir, "no_changes");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SIMPLE_EXECUTABLE_DYNAMIC_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );

    map = openAndMapFileForWrite(file, targetFilePath);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);
    REQUIRE( runElfExecutable(targetFilePath) );
    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
  }

  SECTION("change RUNPATH")
  {
    const QString targetFilePath = makePath(dir, "change_runpath");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SIMPLE_EXECUTABLE_DYNAMIC_WITH_RUNPATH_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );
    REQUIRE( elfFile.dynamicSection().getRunPath().length() >= 4 );

    elfFile.setRunPath( QLatin1String("/tmp") );

    openFileForWrite(file, targetFilePath);
    resizeFile( file, elfFile.minimumSizeToWriteFile() );
    map = mapFile(file);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);
    REQUIRE( runElfExecutable(targetFilePath) );
    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == QLatin1String("/tmp") );
  }

  SECTION("remove RUNPATH")
  {
    const QString targetFilePath = makePath(dir, "remove_runpath");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SIMPLE_EXECUTABLE_DYNAMIC_WITH_RUNPATH_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );
    REQUIRE( elfFile.dynamicSection().containsRunPathEntry() );
    REQUIRE( !elfFile.dynamicSection().getRunPath().isEmpty() );

    elfFile.setRunPath( QString() );

    openFileForWrite(file, targetFilePath);
    resizeFile( file, elfFile.minimumSizeToWriteFile() );
    map = mapFile(file);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);
    REQUIRE( runElfExecutable(targetFilePath) );
    readElfFile(elfFile, targetFilePath);
    REQUIRE( elfFile.seemsValid() );
    REQUIRE( !elfFile.dynamicSection().containsRunPathEntry() );
    REQUIRE( elfFile.dynamicSection().getRunPath().isEmpty() );
    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath).isEmpty() );
  }

  SECTION("set a very long RUNPATH")
  {
    const QString targetFilePath = makePath(dir, "set_very_long_runpath");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SIMPLE_EXECUTABLE_DYNAMIC_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );

    const QString runPath = generateStringWithNChars(10000);
    elfFile.setRunPath(runPath);

    openFileForWrite(file, targetFilePath);
    resizeFile( file, elfFile.minimumSizeToWriteFile() );
    map = mapFile(file);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);

    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( runElfExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == runPath );
  }

  SECTION("set a RUNPATH - the DT_RUNPATH entry does not exist initially")
  {
    const QString targetFilePath = makePath(dir, "add_runpath");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SIMPLE_EXECUTABLE_DYNAMIC_NO_RUNPATH_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );
    REQUIRE( !elfFile.dynamicSection().containsRunPathEntry() );

    elfFile.setRunPath( QLatin1String("/tmp") );

    openFileForWrite(file, targetFilePath);
    resizeFile( file, elfFile.minimumSizeToWriteFile() );
    map = mapFile(file);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);
    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( runElfExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == QLatin1String("/tmp") );
  }
}

TEST_CASE("editRunPath_SharedLibrary")
{
  using Elf::setFileToMap;

  QTemporaryDir dir;
  REQUIRE( dir.isValid() );
  dir.setAutoRemove(true);

  FileWriterFile elfFile;
  QFile file;
  ByteArraySpan map;

  SECTION("set a RUNPATH - the DT_RUNPATH entry does not exist initially")
  {
    const QString targetFilePath = makePath(dir, "add_runpath");
    copyAndReadElfFile(elfFile, targetFilePath, TEST_SHARED_LIBRARY_NO_RUNPATH_FILE_PATH);
    REQUIRE( elfFile.seemsValid() );
    REQUIRE( !elfFile.dynamicSection().containsRunPathEntry() );

    elfFile.setRunPath( QLatin1String("/tmp") );

    openFileForWrite(file, targetFilePath);
    resizeFile( file, elfFile.minimumSizeToWriteFile() );
    map = mapFile(file);
    REQUIRE( !map.isNull() );

    setFileToMap(map, elfFile);

    unmapAndCloseFile(file, map);
    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( lintElfFile(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == QLatin1String("/tmp") );
  }
}

TEST_CASE("sandboxWith_libasan", "[.]")
{
  using Elf::setFileToMap;

  QTemporaryDir dir;
  REQUIRE( dir.isValid() );
  dir.setAutoRemove(false);

  FileWriterFile elfFile;
  QFile file;
  ByteArraySpan map;

  const QString targetFilePath = makePath(dir, "libasan.so");
  copyAndReadElfFile(elfFile, targetFilePath, "/usr/lib/x86_64-linux-gnu/libasan.so.4");
  REQUIRE( elfFile.seemsValid() );

//   REQUIRE( readExecutable(targetFilePath) );
  elfFile.setRunPath( QLatin1String("/tmp") );

  openFileForWrite(file, targetFilePath);
  resizeFile( file, elfFile.minimumSizeToWriteFile() );
  map = mapFile(file);
  REQUIRE( !map.isNull() );

  setFileToMap(map, elfFile);

  unmapAndCloseFile(file, map);
  REQUIRE( readExecutable(targetFilePath) );
  REQUIRE( lintElfFile(targetFilePath) );
  REQUIRE( getExecutableRunPath(targetFilePath) == QLatin1String("/tmp") );
}

TEST_CASE("sandboxWithPatchelf", "[.]")
{
  QTemporaryDir dir;
  REQUIRE( dir.isValid() );
  dir.setAutoRemove(false);

//   qDebug() << "************* file " << targetFilePath << " before changes *************";
//   readExecutable(targetFilePath);
//   qDebug() << "*************************************************************************" ;

  const QString patchelf = QLatin1String("patchelf");

  SECTION("remove RPATH")
  {
    qDebug() << "-----------------  remove RPATH";

    const QString targetFilePath = makePath(dir, "remove_runpath");
    REQUIRE( copyFile(QString::fromLocal8Bit(TEST_SIMPLE_EXECUTABLE_DYNAMIC_WITH_RUNPATH_FILE_PATH), targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );

    const QStringList arguments = QStringList({QLatin1String("--remove-rpath"),QLatin1String("--debug"),targetFilePath});

    REQUIRE( runExecutable(patchelf, arguments) );

    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath).isEmpty() );
  }

  SECTION("set very long RPATH")
  {
    qDebug() << "----------------- set very long RPATH";

    const QString targetFilePath = makePath(dir, "remove_runpath");
    REQUIRE( copyFile(QString::fromLocal8Bit(TEST_SIMPLE_EXECUTABLE_DYNAMIC_FILE_PATH), targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );

    const QString runPath = generateStringWithNChars(10000);
    const QStringList arguments = QStringList({QLatin1String("--set-rpath"),runPath,QLatin1String("--debug"),targetFilePath});

    REQUIRE( runExecutable(patchelf, arguments) );

    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == runPath );
  }

  SECTION("set a RUNPATH - the DT_RUNPATH entry does not exist initially")
  {
    qDebug() << "-----------------  set a RUNPATH - the DT_RUNPATH entry does not exist initially +++++++++++++++++";

    const QString targetFilePath = makePath(dir, "remove_runpath");
    REQUIRE( copyFile(QString::fromLocal8Bit(TEST_SIMPLE_EXECUTABLE_DYNAMIC_NO_RUNPATH_FILE_PATH), targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );

    qDebug() << "************* file " << targetFilePath << " before changes *************";
    readExecutable(targetFilePath);
    qDebug() << "*************************************************************************" ;

//     QStringList arguments = QStringList({QLatin1String("--remove-rpath"),targetFilePath});

//     REQUIRE( runExecutable(patchelf, arguments) );

//     const QString runPath = generateStringWithNChars(10000);
    const QString runPath = QLatin1String("/tmp");
    const QStringList arguments = QStringList({QLatin1String("--set-rpath"),runPath,QLatin1String("--debug"),targetFilePath});

    REQUIRE( runExecutable(patchelf, arguments) );

    REQUIRE( readExecutable(targetFilePath) );
    REQUIRE( runExecutable(targetFilePath) );
    REQUIRE( getExecutableRunPath(targetFilePath) == runPath );
  }
}

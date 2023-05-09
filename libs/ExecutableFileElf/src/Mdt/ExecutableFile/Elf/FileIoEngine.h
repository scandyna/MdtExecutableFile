// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/Algorithm.h"
#include "Mdt/ExecutableFile/ExecutableFileWriteError.h"
#include "Mdt/ExecutableFile/RPath.h"
#include "Mdt/ExecutableFile/RPathElf.h"
#include "Mdt/ExecutableFile/Elf/FileWriter.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderReader.h"
#include "Mdt/ExecutableFile/Elf/SymbolTableReader.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableReader.h"
#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSectionReader.h"
#include "Mdt/ExecutableFile/Elf/GnuHashTableReader.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionReader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterFile.h"
#include <QLatin1Char>

// #include "Debug.h"
// #include <iostream>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class FileIoEngine : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Set the file name
     */
    void setFileName(const QString & name) noexcept
    {
      mFileName = name;
    }

    /*! \brief Clear
     */
    void clear() noexcept
    {
      mFileHeader.clear();
      mSectionNamesStringTableSectionHeader.clear();
      mDynamicSection.clear();
      mFileName.clear();
    }

    /*! \brief Get minimum size to read the file header
     *
     * The get the real minimum size we have to extract the Ident part first.
     * It can be either 52 or 64 bytes.
     * Here we simply return 64.
     *
     * \sa minimumSizeToReadFileHeader(const Ident &)
     */
    int64_t minimumSizeToReadFileHeader() const noexcept
    {
      return 64;
    }

    /*! \brief Get the file header
     */
    FileHeader getFileHeader(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( map.size >= minimumSizeToReadFileHeader() );

      readFileHeaderIfNull(map);

      return mFileHeader;
    }

    /*! \internal
     */
    bool containsDebugSymbols(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);
      readSectionNameStringTableHeaderIfNull(map);

      const auto pred = [](const std::string & currentName){
        return Mdt::ExecutableFile::stringStartsWith(currentName, ".debug");
      };
      const SectionHeader header  = findFirstSectionHeader(map, mFileHeader, mSectionNamesStringTableSectionHeader, SectionType::ProgramData, pred);

      return header.sectionType() != SectionType::Null;
    }

    /*! \brief Get the section header table
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     */
    SectionHeaderTable getSectionHeaderTable(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      SectionHeaderTable table;

      checkFileSizeToReadFileHeader(map);
      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);

      return extractAllSectionHeaders(map, mFileHeader);
    }

    /*! \brief
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     */
    QString getSoName(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      checkFileSizeToReadFileHeader(map);
      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);
      readSectionNameStringTableHeaderIfNull(map);
      readDynamicSectionIfNull(map);

      return mDynamicSection.getSoName();
    }

    /*! \brief
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     */
    QStringList getNeededSharedLibraries(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);
      readSectionNameStringTableHeaderIfNull(map);
      readDynamicSectionIfNull(map);

      return mDynamicSection.getNeededSharedLibraries();
    }

    /*! \brief
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     * \exception RPathFormatError
     */
    RPath getRunPath(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);
      readSectionNameStringTableHeaderIfNull(map);
      readDynamicSectionIfNull(map);

      return RPathElf::rPathFromString( mDynamicSection.getRunPath() );
    }

    /*! \brief
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     */
    void readToFileWriterFile(FileWriterFile & file, const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      readFileHeaderIfNull(map);
      checkFileSizeToReadSectionHeaders(map);
      readSectionNameStringTableHeaderIfNull(map);
      readDynamicSectionIfNull(map);

      FileAllHeaders headers;
      headers.setFileHeader(mFileHeader);
      headers.setProgramHeaderTable( extractAllProgramHeaders(map, mFileHeader) );
      headers.setSectionHeaderTable( extractAllSectionHeaders(map, mFileHeader) );

      file.setHeadersFromFile(headers);
      file.setDynamicSectionFromFile(mDynamicSection);
      file.setSymTabFromFile( extractSymTabPartReferringToSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
      file.setDynSymFromFile( extractDynSymPartReferringToSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
      file.setGotSectionFromFile( extractGotSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
      file.setGotPltSectionFromFile( extractGotPltSection( map, headers.fileHeader(), headers.sectionHeaderTable() ) );

      if( headers.containsProgramInterpreterSectionHeader() ){
        file.setProgramInterpreterSectionFromFile( extractProgramInterpreterSection( map, headers.programInterpreterSectionHeader() ) );
      }

      if( headers.containsGnuHashTableSectionHeader() ){
        file.setGnuHashTableSection( GnuHashTableReader::extractHasTable( map, headers.fileHeader(), headers.gnuHashTableSectionHeader() ) );
      }

      try{
        file.setNoteSectionTableFromFile( NoteSectionReader::extractNoteSectionTable( map, headers.fileHeader(), headers.sectionHeaderTable() ) );
      }catch(const NoteSectionReadError & error){
        const QString msg = tr("file '%1' contains a invalid note section: %2")
                            .arg( mFileName, error.whatQString() );
        throw ExecutableFileReadError(msg);
      }
    }

    /*! \brief
     *
     * \pre \a map must not be null
     * \exception ExecutableFileWriteError
     */
    void setFileWriterToMap(ByteArraySpan map, const FileWriterFile & file)
    {
      assert( !map.isNull() );
      assert( map.size >= file.minimumSizeToWriteFile() );

      setFileToMap(map, file);
    }

   private:

    /*! \brief Check if given map size is enought to read the file header
     *
     * \pre \a map must not be null
     * \exception ExecutableFileReadError
     */
    void checkFileSizeToReadFileHeader(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      if( map.size < minimumSizeToReadFileHeader() ){
        const QString message = tr("file '%1' is to small to read the file header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    void readFileHeaderIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( map.size >= minimumSizeToReadFileHeader() );

      if( mFileHeader.seemsValid() ){
        return;
      }

      mFileHeader = extractFileHeader(map);
      if( !mFileHeader.seemsValid() ){
        const QString message = tr("file '%1' does not contain a valid file header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    /*! \brief
     *
     * \pre mFileHeader must be valid
     */
    void checkFileSizeToReadSectionHeaders(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mFileHeader.seemsValid() );

      if( map.size < mFileHeader.minimumSizeToReadAllSectionHeaders() ){
        const QString message = tr("file '%1' is to small to read section headers")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    /*! \brief
     *
     * \pre mFileHeader must be valid
     */
    void readSectionNameStringTableHeaderIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mFileHeader.seemsValid() );

      if( headerIsStringTableSection(mSectionNamesStringTableSectionHeader) ){
        return;
      }

      mSectionNamesStringTableSectionHeader = extractSectionNameStringTableHeader(map, mFileHeader);
      if( mSectionNamesStringTableSectionHeader.sectionType() == SectionType::Null ){
        const QString message = tr("file '%1' does not contain the section names string table section header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    /*! \brief
     */
    void readDynamicSectionIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mFileHeader.seemsValid() );
      assert( headerIsStringTableSection(mSectionNamesStringTableSectionHeader) );

      if( !mDynamicSection.isNull() ){
        return;
      }

      try{
        mDynamicSection = extractDynamicSection(map, mFileHeader, mSectionNamesStringTableSectionHeader);
      }catch(const DynamicSectionReadError & error){
        const QString message = tr("file '%1': error while reading the .dynamic section: %2")
                                .arg( mFileName, error.whatQString() );
        throw ExecutableFileReadError(message);
      }catch(const StringTableError & error){
        const QString message = tr("file '%1': error while reading the string table for the .dynamic section: %2")
                                .arg( mFileName, error.whatQString() );
        throw ExecutableFileReadError(message);
      }

      if( mDynamicSection.isNull() ){
        const QString message = tr("file '%1' does not contain the .dynamic section")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    FileHeader mFileHeader;
    SectionHeader mSectionNamesStringTableSectionHeader;
    DynamicSection mDynamicSection;
    QString mFileName;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_IO_ENGINE_H

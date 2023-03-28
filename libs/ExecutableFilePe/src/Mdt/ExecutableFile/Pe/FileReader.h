// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PE_FILE_READER_H
#define MDT_EXECUTABLE_FILE_PE_FILE_READER_H

#include "Mdt/ExecutableFile/Pe/FileHeader.h"
#include "Mdt/ExecutableFile/Pe/SectionHeader.h"
#include "Mdt/ExecutableFile/Pe/ImportDirectory.h"
#include "Mdt/ExecutableFile/Pe/Exceptions.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "Mdt/ExecutableFile/ExecutableFileReaderUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include <QCoreApplication>
#include <QtGlobal>
#include <QString>
#include <QStringRef>
#include <QStringList>
#include <QObject>
#include <string>
#include <cassert>

// #include "Debug.h"
// #include <iostream>
// #include <QDebug>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

  /*! \internal
   */
  inline
  QString tr(const char *sourceText) noexcept
  {
    assert( sourceText != nullptr );

    return QCoreApplication::translate("Mdt::ExecutableFile::Pe::FileReader", sourceText);
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map size must be >= 64 (0x3C + 4)
   */
  inline
  DosHeader extractDosHeader(const ByteArraySpan & map) noexcept
  {
    assert( !map.isNull() );
    assert( map.size >= 64 );

    DosHeader header;

    header.peSignatureOffset = get32BitValueLE( map.subSpan(0x3C, 4) );

    return header;
  }

  /*! \internal
   *
   * \pre \a header must be valid
   */
  inline
  int64_t minimumSizeToExtractPeSignature(const DosHeader & header) noexcept
  {
    assert( header.seemsValid() );

    return header.peSignatureOffset + 4;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a dosHeader must be valid
   * \pre \a map size must be big enouth to extract the PE signature
   */
  inline
  bool containsPeSignature(const ByteArraySpan & map, const DosHeader & dosHeader) noexcept
  {
    assert( !map.isNull() );
    assert( dosHeader.seemsValid() );
    assert( map.size >= minimumSizeToExtractPeSignature(dosHeader) );

    return arraysAreEqual( map.data + dosHeader.peSignatureOffset, 4, {'P','E',0,0} );
  }

  /*! \internal
   *
   * \pre \a dosHeader must be valid
   */
  inline
  int64_t minimumSizeToExtractCoffHeader(const DosHeader & dosHeader) noexcept
  {
    assert( dosHeader.seemsValid() );

    return minimumSizeToExtractPeSignature(dosHeader) + 20;
  }

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array size must be 20
   */
  inline
  CoffHeader coffHeaderFromArray(const ByteArraySpan & array) noexcept
  {
    assert( !array.isNull() );
    assert( array.size == 20 );

    CoffHeader header;

    header.machine = get16BitValueLE(array);
    header.numberOfSections = get16BitValueLE( array.subSpan(2, 2) );
    header.timeDateStamp = get32BitValueLE( array.subSpan(4, 4) );
    header.pointerToSymbolTable = get32BitValueLE( array.subSpan(8, 4) );
    header.numberOfSymbols = get32BitValueLE( array.subSpan(12, 4) );
    header.sizeOfOptionalHeader = get16BitValueLE( array.subSpan(16, 2) );
    header.characteristics = get16BitValueLE( array.subSpan(18, 2) );

    return header;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a dosHeader must be valid
   * \pre \a map must contain the PE signature
   * \pre \a map size must be big enouth to extract the COFF header
   */
  inline
  CoffHeader extractCoffHeader(const ByteArraySpan & map, const DosHeader & dosHeader) noexcept
  {
    assert( !map.isNull() );
    assert( dosHeader.seemsValid() );
    assert( containsPeSignature(map, dosHeader) );
    assert( map.size >= minimumSizeToExtractCoffHeader(dosHeader) );

    return coffHeaderFromArray( map.subSpan(dosHeader.peSignatureOffset + 4, 20) );
  }

  /*! \internal
   *
   * \pre \a coffHeader must be valid
   */
  inline
  int64_t minimumSizeToExtractOptionalHeader(const CoffHeader & coffHeader, const DosHeader & dosHeader) noexcept
  {
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );

    return minimumSizeToExtractCoffHeader(dosHeader) + coffHeader.sizeOfOptionalHeader;
  }

  /*! \internal
   *
   * \pre \a dosHeader must be valid
   */
  inline
  int64_t optionalHeaderOffset(const DosHeader & dosHeader) noexcept
  {
    assert( dosHeader.seemsValid() );

    return dosHeader.peSignatureOffset + 24;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a coffHeader must be valid
   * \pre \a map size must be coffHeader's sizeOfOptionalHeader
   * \exception InvalidMagicType
   */
  inline
  OptionalHeader optionalHeaderFromArray(const ByteArraySpan & map, const CoffHeader & coffHeader)
  {
    assert( !map.isNull() );
    assert( coffHeader.seemsValid() );
    assert( map.size == static_cast<int64_t>(coffHeader.sizeOfOptionalHeader) );
    Q_UNUSED(coffHeader);

    OptionalHeader header;

    header.magic = get16BitValueLE(map);

    if( header.magicType() == MagicType::Pe32 ){
      header.numberOfRvaAndSizes = get32BitValueLE( map.subSpan(92, 4) );
      header.importTable = get64BitValueLE( map.subSpan(104, 8) );
      if( map.size > 151  ){
        header.debug = get64BitValueLE( map.subSpan(144, 8) );
      }
      if( map.size >= 208 ){
        header.delayImportTable = get64BitValueLE( map.subSpan(200, 8) );
      }
    }else if( header.magicType() == MagicType::Pe32Plus ){
      header.numberOfRvaAndSizes = get32BitValueLE( map.subSpan(108, 4) );
      if( map.size >= 128 ){
        header.importTable = get64BitValueLE( map.subSpan(120, 8) );
      }
      if( map.size > 168  ){
        header.debug = get64BitValueLE( map.subSpan(160, 8) );
      }
      if( map.size >= 224 ){
        header.delayImportTable = get64BitValueLE( map.subSpan(216, 8) );
      }
    }else{
      const QString message = tr("invalid or unsupported magic type: %1")
                              .arg( static_cast<int>(header.magic) );
      throw InvalidMagicType(message);
    }

    return header;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a coffHeader must be valid
   * \pre \a dosHeader must be valid
   * \pre \a map size must be big enouth to extract the optional header
   * \exception InvalidMagicType
   */
  inline
  OptionalHeader extractOptionalHeader(const ByteArraySpan & map, const CoffHeader & coffHeader, const DosHeader & dosHeader)
  {
    assert( !map.isNull() );
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );
    assert( map.size >= minimumSizeToExtractOptionalHeader(coffHeader, dosHeader) );

    return optionalHeaderFromArray(map.subSpan(optionalHeaderOffset(dosHeader), coffHeader.sizeOfOptionalHeader), coffHeader);
  }

  /*! \internal Get a string from a array of unsigned characters
   *
   * Will read from \a charArray until a null char is encountered,
   * or until the end of the array.
   * This means that this function accepts non null terminated strings.
   *
   * \pre \a charArray must not be null
   * \sa https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#section-table-section-headers
   */
  inline
  QString qStringFromUft8BoundedUnsignedCharArray(const ByteArraySpan & charArray) noexcept
  {
    assert( !charArray.isNull() );

    if( charArray.data[charArray.size-1] != 0 ){
      return QString::fromUtf8( reinterpret_cast<const char*>(charArray.data), static_cast<int>(charArray.size) );
    }

    return QString::fromUtf8( reinterpret_cast<const char*>(charArray.data) );
  }

  /*! \internal Get a string from a array of unsigned characters
   *
   * The PE specification seems not to say anything about unicode encoding.
   * To get the DLL names from import directories, PE tells that it is ASCII.
   * So, assume UTF-8 and hope..
   * (note: using platform specific encoding can be problematic
   *        for cross-compilation)
   *
   * \pre \a charArray must not be null
   * \exception NotNullTerminatedStringError
   */
  inline
  QString qStringFromUft8ByteArraySpan(const ByteArraySpan & charArray)
  {
    assert( !charArray.isNull() );

    return qStringFromUft8UnsignedCharArray(charArray);
  }

  /*! \internal
   */
  inline
  int64_t minimumSizeToExtractCoffStringTableHandle(const CoffHeader & coffHeader) noexcept
  {
    assert( coffHeader.seemsValid() );

    return coffHeader.coffStringTableOffset() + 4;
  }

  /*! \internal
   *
   * \exception FileCorrupted
   */
  inline
  CoffStringTableHandle extractCoffStringTableHandle(const ByteArraySpan & map, const CoffHeader & coffHeader)
  {
    assert( !map.isNull() );
    assert( coffHeader.seemsValid() );
    assert( coffHeader.containsCoffStringTable() );
    assert( map.size >= minimumSizeToExtractCoffStringTableHandle(coffHeader) );

    CoffStringTableHandle stringTable;

    const int64_t stringTableByteCount = get32BitValueLE( map.subSpan(coffHeader.coffStringTableOffset(), 4) );
    if( !map.isInRange(coffHeader.coffStringTableOffset(), stringTableByteCount) ){
      const QString message = tr("declared COFF string table size %1 is out of range of the file size %2")
                              .arg( QString::number(stringTableByteCount), QString::number(map.size) );
      throw FileCorrupted(message);
    }
    stringTable.table = map.subSpan(coffHeader.coffStringTableOffset(), stringTableByteCount);

    return stringTable;
  }

  /*! \internal
   *
   * \exception NotNullTerminatedStringError
   */
  inline
  QString extractString(const CoffStringTableHandle & stringTable, int offset) noexcept
  {
    assert( !stringTable.isEmpty() );
    assert( offset >= 0 );
    assert( stringTable.isInRange(offset) );

    return qStringFromUft8ByteArraySpan( stringTable.table.subSpan(offset) );
  }

  /*! \internal
   *
   * \pre \a coffHeader must be valid
   * \pre \a dosHeader must be valid
   */
  inline
  int64_t sectionTableOffset(const CoffHeader & coffHeader, const DosHeader & dosHeader) noexcept
  {
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );

    return optionalHeaderOffset(dosHeader) + coffHeader.sizeOfOptionalHeader;
  }

  /*! \internal
   *
   * \pre \a coffHeader must be valid
   */
  inline
  int64_t sectionTableSize(const CoffHeader & coffHeader) noexcept
  {
    assert( coffHeader.seemsValid() );

    return coffHeader.numberOfSections * 40;
  }

  /*! \internal
   *
   * \pre \a coffHeader must be valid
   * \pre \a dosHeader must be valid
   */
  inline
  int64_t minimumSizeToExtractSectionTable(const CoffHeader & coffHeader, const DosHeader & dosHeader) noexcept
  {
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );

    return sectionTableOffset(coffHeader, dosHeader) + sectionTableSize(coffHeader);
  }

  /*! \internal
   *
   * The section header name is directly encoded as UTF-8 null padded string.
   *
   * For names longer that 8 bytes, it begins with a '/'
   * followed by an ASCII representation of a decimal number
   * that is an offset into the COFF string table.
   *
   * Microsoft's documentations tells that the COFF string table
   * is not used for executable image files.
   * Despite that, some compilers, like Gcc,
   * will use names longer thant 8 bytes,
   * generate a COFF string table and put a offset to the name
   * (for example '/81').
   *
   * \sa https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#section-table-section-headers
   *
   * \pre \a charArray must not be null
   * \pre \a charArray size must be 8
   * \exception NotNullTerminatedStringError
   * \exception FileCorrupted
   */
  inline
  QString getSectionHeaderName(const ByteArraySpan & charArray, const CoffStringTableHandle & stringTable)
  {
    assert( !charArray.isNull() );
    assert( charArray.size == 8 );

    const QString name = qStringFromUft8BoundedUnsignedCharArray(charArray);

    if( (charArray.data[0] == '/') && !stringTable.isEmpty() ){
      bool numOk = false;
      const int offset = name.rightRef(name.size()-1).toInt(&numOk);
      if( !numOk || (offset < 4) || !stringTable.isInRange(offset) ){
        const QString message = tr("section %1 is a invalid offset to the COFF string table")
                                .arg(name);
        throw FileCorrupted(message);
      }

      return extractString(stringTable, offset);
    }

    return name;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map size must be the size of a section header
   * \note \a stringTable can be empty
   * \exception NotNullTerminatedStringError
   * \exception FileCorrupted
   */
  inline
  SectionHeader sectionHeaderFromArray(const ByteArraySpan & map, const CoffStringTableHandle & stringTable)
  {
    assert( !map.isNull() );
    assert( map.size == 40 );

    SectionHeader header;

    header.name = getSectionHeaderName( map.subSpan(0, 8), stringTable );
    header.virtualSize = get32BitValueLE( map.subSpan(8, 4) );
    header.virtualAddress = get32BitValueLE( map.subSpan(12, 4) );
    header.sizeOfRawData = get32BitValueLE( map.subSpan(16, 4) );
    header.pointerToRawData = get32BitValueLE( map.subSpan(20, 4) );

    return header;
  }

  /*! \internal Find the first section header that matches \a predicate
   *
   * Predicate is of the form:
   * \code
   * bool p(const SectionHeader & header);
   * \endcode
   */
  template<typename UnaryPredicate>
  inline
  SectionHeader findFirstSectionHeader(const ByteArraySpan & map, const CoffHeader & coffHeader, const DosHeader & dosHeader, UnaryPredicate predicate)
  {
    assert( !map.isNull() );
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );
    assert( map.size >= minimumSizeToExtractSectionTable(coffHeader, dosHeader) );

    CoffStringTableHandle stringTable;
    if( coffHeader.containsCoffStringTable() && map.size >= minimumSizeToExtractCoffStringTableHandle(coffHeader) ){
      stringTable = extractCoffStringTableHandle(map, coffHeader);
    }

    int64_t offset = sectionTableOffset(coffHeader, dosHeader);
    for(uint16_t i = 1; i < coffHeader.numberOfSections; ++i){
      const SectionHeader sectionHeader = sectionHeaderFromArray( map.subSpan(offset, 40), stringTable );
      if( sectionHeader.seemsValid() && predicate(sectionHeader) ){
        return sectionHeader;
      }
      offset += 40;
    }

    return SectionHeader();
  }

  /*! \internal
   *
   * \exception NotNullTerminatedStringError
   * \exception FileCorrupted
   */
  inline
  SectionHeader findSectionHeaderByRva(const ByteArraySpan & map, uint32_t rva, const CoffHeader & coffHeader, const DosHeader & dosHeader)
  {
    assert( !map.isNull() );
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );
    assert( map.size >= minimumSizeToExtractSectionTable(coffHeader, dosHeader) );

    const auto pred = [rva](const SectionHeader & header){
      return header.rvaIsInThisSection(rva);
    };

    return findFirstSectionHeader(map, coffHeader, dosHeader, pred);
  }

  /*! \internal Find a section header from a optional header data directory
   *
   * \pre \a map must not be null
   * \pre \a directory must not be null
   * \pre \a coffHeader must be valid
   * \pre \a dosHeader must be valid
   * \pre \a map size must be big enouth to extract the the section table
   * \exception NotNullTerminatedStringError
   * \exception FileCorrupted
   */
  inline
  SectionHeader findSectionHeader(const ByteArraySpan & map, const ImageDataDirectory & directory, const CoffHeader & coffHeader, const DosHeader & dosHeader)
  {
    assert( !map.isNull() );
    assert( !directory.isNull() );
    assert( coffHeader.seemsValid() );
    assert( dosHeader.seemsValid() );
    assert( map.size >= minimumSizeToExtractSectionTable(coffHeader, dosHeader) );

    return findSectionHeaderByRva(map, directory.virtualAddress, coffHeader, dosHeader);
  }

  /*! \internal
   */
  inline int64_t minimumSizeToExtractSection(const SectionHeader & sectionHeader, const ImageDataDirectory & directory) noexcept
  {
    assert( sectionHeader.seemsValid() );
    assert( !directory.isNull() );
    assert( sectionHeader.rvaIsValid(directory.virtualAddress) );

    return sectionHeader.rvaToFileOffset(directory.virtualAddress) + directory.size;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map size must be 20
   */
  inline
  ImportDirectory importDirectoryFromArray(const ByteArraySpan & map) noexcept
  {
    assert( !map.isNull() );
    assert( map.size == 20 );

    ImportDirectory directory;

    directory.nameRVA = get32BitValueLE( map.subSpan(12, 4) );

    return directory;
  }

  /*! \internal
   */
  inline
  ImportDirectoryTable importDirectoryTableFromArray(const ByteArraySpan & map) noexcept
  {
    assert( !map.isNull() );
    assert( map.size >= 20 );

    ImportDirectoryTable table;

    /*
     * We must not extract the whole section,
     * but only the import directory table.
     * This table ends with a null directory.
     *
     * see https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#the-idata-section
     */
    const int64_t lastOffset = map.size - 20;
    for( int64_t offset = 0; offset <= lastOffset; offset += 20 ){
      const ImportDirectory directory = importDirectoryFromArray( map.subSpan(offset, 20) );
      if( directory.isNull() ){
        return table;
      }
      table.push_back(directory);
    }

    return table;
  }

  /*! \internal
   */
  inline
  ImportDirectoryTable extractImportDirectoryTable(const ByteArraySpan & map, const SectionHeader & sectionHeader, const ImageDataDirectory & directory) noexcept
  {
    assert( !map.isNull() );
    assert( sectionHeader.seemsValid() );
    assert( !directory.isNull() );
    assert( sectionHeader.rvaIsValid(directory.virtualAddress) );
    assert( map.size >= minimumSizeToExtractSection(sectionHeader, directory) );

    const int64_t offset = sectionHeader.rvaToFileOffset(directory.virtualAddress);
    const int64_t size = directory.size;

    return importDirectoryTableFromArray( map.subSpan(offset, size) );
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map size must be 32
   */
  inline
  DelayLoadDirectory delayLoadDirectoryFromArray(const ByteArraySpan & map) noexcept
  {
    assert( !map.isNull() );
    assert( map.size == 32 );

    DelayLoadDirectory directory;

    directory.attributes = get32BitValueLE( map.subSpan(0, 4) );
    directory.nameRVA = get32BitValueLE( map.subSpan(4, 4) );

    return directory;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * 
   */
  inline
  DelayLoadTable delayLoadTableFromArray(const ByteArraySpan & map) noexcept
  {
    assert( !map.isNull() );
    assert( map.size >= 32 );

    DelayLoadTable table;

    /*
     * We must not extract the whole section,
     * but only the delay load table.
     * This table ends with a null directory.
     *
     * see https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#the-idata-section
     */
    const int64_t lastOffset = map.size - 32;
    for( int64_t offset = 0; offset <= lastOffset; offset += 32 ){
      const DelayLoadDirectory directory = delayLoadDirectoryFromArray( map.subSpan(offset, 32) );
      if( directory.isNull() ){
        return table;
      }
      table.push_back(directory);
    }

    return table;
  }

  /*! \internal
   */
  inline
  DelayLoadTable extractDelayLoadTable(const ByteArraySpan & map, const SectionHeader & sectionHeader, const ImageDataDirectory & directory) noexcept
  {
    assert( !map.isNull() );
    assert( sectionHeader.seemsValid() );
    assert( !directory.isNull() );
    assert( sectionHeader.rvaIsValid(directory.virtualAddress) );
    assert( map.size >= minimumSizeToExtractSection(sectionHeader, directory) );

    const int64_t offset = sectionHeader.rvaToFileOffset(directory.virtualAddress);
    const int64_t size = directory.size;

    return delayLoadTableFromArray( map.subSpan(offset, size) );
  }


  /*! \internal
   */
  class FileReader : public QObject
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
      mFileName.clear();
      mDosHeader.clear();
      mCoffHeader.clear();
      mOptionalHeader.clear();
    }

    const CoffHeader & coffHeader() const noexcept
    {
      assert( mCoffHeader.seemsValid() );

      return mCoffHeader;
    }

    QStringList getNeededSharedLibraries(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      QStringList dlls;

      extractDosHeaderIfNull(map);
      extractCoffHeaderIfNull(map);
      extractOptionalHeaderIfNull(map);

      if( mOptionalHeader.containsImportTable() ){
        const ImageDataDirectory directoryDescriptor = mOptionalHeader.importTableDirectory();
        const SectionHeader sectionHeader = findSectionHeader(map, directoryDescriptor, mCoffHeader, mDosHeader);
        if( !sectionHeader.seemsValid() ){
          const QString message = tr("file '%1' declares to have a import table, but related section could not be found")
                                  .arg(mFileName);
          throw ExecutableFileReadError(message);
        }

        if( !sectionHeader.rvaIsValid(directoryDescriptor.virtualAddress) ){
          const QString message = tr("file '%1' the import directory descriptor contains a invalid address to its section")
                                  .arg(mFileName);
          throw ExecutableFileReadError(message);
        }
        const ImportDirectoryTable importTable = extractImportDirectoryTable(map, sectionHeader, directoryDescriptor);
        for(const auto & directory : importTable){
          dlls.push_back( extractDllName(map, sectionHeader, directory) );
        }
      }

      if( mOptionalHeader.containsDelayImportTable() ){
        const ImageDataDirectory directoryDescriptor = mOptionalHeader.delayImportTableDirectory();
        const SectionHeader sectionHeader = findSectionHeader(map, directoryDescriptor, mCoffHeader, mDosHeader);
        if( !sectionHeader.seemsValid() ){
          const QString message = tr("file '%1' declares to have delay load table, but related section could not be found")
                                  .arg(mFileName);
          throw ExecutableFileReadError(message);
        }
        if( !sectionHeader.rvaIsValid(directoryDescriptor.virtualAddress) ){
          const QString message = tr("file '%1' the delay load directory descriptor contains a invalid address to its section")
                                  .arg(mFileName);
          throw ExecutableFileReadError(message);
        }
        const DelayLoadTable delayLoadTable = extractDelayLoadTable(map, sectionHeader, directoryDescriptor);

        for(const auto & directory : delayLoadTable){
          dlls.push_back( extractDllName(map, sectionHeader, directory) );
        }
      }

      return dlls;
    }

    bool tryExtractDosHeader(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( map.size >= 64 );

      mDosHeader = extractDosHeader(map);

      return mDosHeader.seemsValid();
    }

    int64_t minimumSizeToExtractCoffHeader() const noexcept
    {
      assert( mDosHeader.seemsValid() );

      return Mdt::ExecutableFile::Pe::minimumSizeToExtractCoffHeader(mDosHeader);
    }

    bool tryExtractCoffHeader(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mDosHeader.seemsValid() );
      assert( map.size >= minimumSizeToExtractCoffHeader() );

      mCoffHeader = extractCoffHeader(map, mDosHeader);

      return mCoffHeader.seemsValid();
    }

    int64_t minimumSizeToExtractOptionalHeader() const noexcept
    {
      assert( mDosHeader.seemsValid() );
      assert( mCoffHeader.seemsValid() );

      return Mdt::ExecutableFile::Pe::minimumSizeToExtractOptionalHeader(mCoffHeader, mDosHeader);
    }

    bool tryExtractOptionalHeader(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mDosHeader.seemsValid() );
      assert( mCoffHeader.seemsValid() );
      assert( map.size >= minimumSizeToExtractOptionalHeader() );

      mOptionalHeader = extractOptionalHeader(map, mCoffHeader, mDosHeader);

      return mOptionalHeader.seemsValid();
    }

    bool isSharedLibrary() const noexcept
    {
      assert( mCoffHeader.seemsValid() );

      return mCoffHeader.isDll();
    }

    bool isValidExecutableImage() const noexcept
    {
      assert( mCoffHeader.seemsValid() );

      return mCoffHeader.isValidExecutableImage();
    }

    bool containsDebugSymbols(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      extractDosHeaderIfNull(map);
      extractCoffHeaderIfNull(map);
      extractOptionalHeaderIfNull(map);

      assert( map.size >= minimumSizeToExtractSectionTable(mCoffHeader, mDosHeader) );

      if( mOptionalHeader.containsDebugDirectory() ){
        return true;
      }

      const auto pred = [](const SectionHeader & header){
        return header.name.startsWith( QLatin1String(".debug") );
      };
      try{
        const SectionHeader debugSectionHeader = findFirstSectionHeader(map, mCoffHeader, mDosHeader, pred);
        if( debugSectionHeader.seemsValid() ){
          return true;
        }
      }catch(const FileCorrupted & error){
        const QString message = tr("file '%1' is corruped: %2")
                                .arg( mFileName, error.whatQString() );
        throw ExecutableFileReadError(message);
      }

      if( mCoffHeader.isDebugStripped() ){
        return false;
      }

      return false;
    }

   private:

    void extractDosHeaderIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );

      if( map.size < 64 ){
        const QString message = tr("file '%1' is to small to be a PE file")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }

      if( mDosHeader.seemsValid() ){
        return ;
      }
      if( !tryExtractDosHeader(map) ){
        const QString message = tr("file '%1' does not contain the DOS header (no access to PE signature)")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    void extractCoffHeaderIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mDosHeader.seemsValid() );

      if( mCoffHeader.seemsValid() ){
        return;
      }

      if( map.size < minimumSizeToExtractCoffHeader() ){
        const QString message = tr("file '%1' is to small to extract the COFF header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
      if( !tryExtractCoffHeader(map) ){
        const QString message = tr("file '%1' does not contain the COFF header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    void extractOptionalHeaderIfNull(const ByteArraySpan & map)
    {
      assert( !map.isNull() );
      assert( mDosHeader.seemsValid() );
      assert( mCoffHeader.seemsValid() );

      if( mOptionalHeader.seemsValid() ){
        return;
      }

      if( map.size < minimumSizeToExtractOptionalHeader() ){
        const QString message = tr("file '%1' is to small to extract the Optional header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
      if( !tryExtractOptionalHeader(map) ){
        const QString message = tr("file '%1' does not contain the Optional header")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    QString extractDllNameByRva(const ByteArraySpan & map, uint32_t rva, const SectionHeader & candidateSectionHeader)
    {
      assert( !map.isNull() );
      assert( candidateSectionHeader.seemsValid() );
      assert( mDosHeader.seemsValid() );
      assert( mCoffHeader.seemsValid() );

      SectionHeader sectionHeader;
      if( candidateSectionHeader.rvaIsInThisSection(rva) ){
        sectionHeader = candidateSectionHeader;
      }else{
        sectionHeader = findSectionHeaderByRva(map, rva, mCoffHeader, mDosHeader);
      }
      if( !sectionHeader.seemsValid() ){
        const QString message = tr("file '%1': extracting DLL name failed, could not find a section header for RVA 0x%2")
                                .arg( mFileName, QString::number(rva, 16) );
        throw ExecutableFileReadError(message);
      }
      assert( sectionHeader.rvaIsInThisSection(rva) );

      const int64_t offset = sectionHeader.rvaToFileOffset(rva);
      if( map.size <= offset ){
        const QString message = tr("file '%1' is to small to extract a DLL name from import or delay load directory")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
      try{
        return qStringFromUft8ByteArraySpan( map.subSpan(offset) );
      }catch(const NotNullTerminatedStringError &){
        const QString message = tr("file '%1' failed to extract a DLL name from import or delay load directory (no end of string found)")
                                .arg(mFileName);
        throw ExecutableFileReadError(message);
      }
    }

    /*! \internal
     */
    inline
    QString extractDllName(const ByteArraySpan & map, const SectionHeader & candidateSectionHeader, const ImportDirectory & directory)
    {
      assert( !map.isNull() );
      assert( candidateSectionHeader.seemsValid() );
      assert( !directory.isNull() );

      return extractDllNameByRva(map, directory.nameRVA, candidateSectionHeader);
    }

    /*! \internal
     */
    inline
    QString extractDllName(const ByteArraySpan & map, const SectionHeader & candidateSectionHeader, const DelayLoadDirectory & directory)
    {
      assert( !map.isNull() );
      assert( candidateSectionHeader.seemsValid() );
      assert( !directory.isNull() );

      return extractDllNameByRva(map, directory.nameRVA, candidateSectionHeader);
    }

    DosHeader mDosHeader;
    CoffHeader mCoffHeader;
    OptionalHeader mOptionalHeader;
    QString mFileName;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{

#endif // #ifndef MDT_EXECUTABLE_FILE_PE_FILE_READER_H

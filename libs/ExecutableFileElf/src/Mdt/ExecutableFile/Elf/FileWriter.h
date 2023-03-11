// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_H

#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/OffsetRange.h"
#include "Mdt/ExecutableFile/Elf/FileWriterFile.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeadersWriter.h"
#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include "Mdt/ExecutableFile/Elf/DynamicSectionWriter.h"
#include "Mdt/ExecutableFile/Elf/StringTable.h"
#include "Mdt/ExecutableFile/Elf/StringTableWriter.h"
#include "Mdt/ExecutableFile/Elf/SymbolTableWriter.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableWriter.h"
#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSectionWriter.h"
#include "Mdt/ExecutableFile/Elf/GnuHashTableWriter.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionWriter.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <QtEndian>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cassert>

// #include <iostream>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal Shift bytes starting from \a startOffset towards the beginning for \a count bytes
   *
   * Example:
   * \code
   * // Initial map
   * {s,t,r,\0,o,l,d,\0,1,2,3}
   *
   * shiftBytesToBegin(map, 8, 4);
   *
   * // map after shift
   * {s,t,r,\0,1,2,3}
   * \endcode
   */
  inline
  void shiftBytesToBegin(ByteArraySpan & map, uint64_t startOffset, int64_t count) noexcept
  {
    assert( !map.isNull() );
    assert( static_cast<uint64_t>(map.size) > startOffset );
    assert( map.size >= count );
    assert( count >= 0 );

    /*
     * 1 element:
     * f.ex. {A}
     * we have nothing to move
     */
    if(map.size <= 1){
      map.size = 0;
      return;
    }

    const auto first = map.begin() + startOffset;
    const auto last = map.end();
    const auto dFirst = first - count;

    std::move(first, last, dFirst);

    map.size -= count;
    assert( map.size >= 0 );
  }

  /*! \internal Shift bytes starting from \a startOffset towards the end for \a count bytes
   *
   * Example:
   * \code
   * // Initial map
   * {s,t,r,\0,1,2,3,0,0,0}
   *
   * shiftBytesToEnd(map, 4, 3);
   *
   * // map after shift
   * {s,t,r,\0,?,?,?,1,2,3}
   * \endcode
   *
   * Note that \a map should have enought space to shift the byte by \a count ,
   * otherwise some data can be lost:
   * \code
   * {s,t,r,\0,1,2,3,0}
   *
   * shiftBytesToEnd(map, 4, 3);
   *
   * // map after shift
   * {s,t,r,\0,?,?,?,1}
   * \endcode
   */
  inline
  void shiftBytesToEnd(ByteArraySpan & map, uint64_t startOffset, int64_t count) noexcept
  {
    assert( !map.isNull() );
    assert( count >= 0 );
    assert( static_cast<uint64_t>(map.size) > startOffset );
    assert( map.size >= count );

    auto first = map.begin() + startOffset;
    auto last = map.end() - count;
    auto dFirst = map.end();

    std::move_backward(first, last, dFirst);
  }


  /*! \internal
   */
  inline
  void replaceBytes(ByteArraySpan map, const OffsetRange & range, unsigned char c) noexcept
  {
    assert( !map.isNull() );
    assert( map.size >= range.minimumSizeToAccessRange() );

    auto first = map.begin() + range.begin();
    auto last =  map.begin() + range.end();

    while(first != last){
      *first = c;
      ++first;
    }
  }

  /*! \internal
   *
   * Old string table:
   * \code
   * \0libA.so\0/home/me/lib\0
   * \endcode
   *
   * New string table:
   * \code
   * \0libA.so\0
   * \endcode
   *
   * Because we don't move any section or segment after the string table,
   * there will be a hole that contains the old strings,
   * despite they are not referenced anymore:
   * \code
   * // Old strings remains
   * \0libA.so\0/home/me/lib\0
   *
   * // replace them
   * \0libA.so\0\0\0\0\0\0\0\0\0\0\0\0\0\0
   * \endcode
   */
  inline
  void setBytesAfterOldDynamicStringTableNull(ByteArraySpan map, const FileWriterFile & file) noexcept
  {
    assert( !map.isNull() );

    const uint64_t begin = file.dynamicStringTableOffsetRange().end();
    const uint64_t end = file.originalDynamicStringTableOffsetRange().end();

    if(begin < end){
      const auto range = OffsetRange::fromBeginAndEndOffsets(begin, end);
      replaceBytes(map, range, '\0');
    }
  }

  /*! \internal
   */
  inline
  void setFileToMap(ByteArraySpan map, const FileWriterFile & file) noexcept
  {
    assert( !map.isNull() );
    assert( file.seemsValid() );
    assert( map.size >= file.minimumSizeToWriteFile() );

    if( file.dynamicStringTableMovesToEnd() ){
      replaceBytes(map, file.originalDynamicStringTableOffsetRange(), '\0');
    }else{
      setBytesAfterOldDynamicStringTableNull(map, file);
    }

    if( file.dynamicSectionMovesToEnd() ){
      if( !file.gotSection().isEmpty() && file.headers().containsGotSectionHeader() ){
        setGlobalOffsetTableToMap( map, file.headers().gotSectionHeader(), file.gotSection(), file.fileHeader() );
      }
      if( !file.gotPltSection().isEmpty() && file.headers().containsGotPltSectionHeader() ){
        setGlobalOffsetTableToMap( map, file.headers().gotPltSectionHeader(), file.gotPltSection(), file.fileHeader() );
      }
    }

    if( file.headers().containsProgramInterpreterSectionHeader() ){
      setProgramInterpreterSectionToMap( map, file.headers().programInterpreterSectionHeader(), file.programInterpreterSection() );
    }

    if( file.headers().containsGnuHashTableSectionHeader() ){
      GnuHashTableWriter::setGnuHashTableToMap( map, file.headers().gnuHashTableSectionHeader(), file.gnuHashTableSection(), file.fileHeader() );
    }

    NoteSectionWriter::setNoteSectionTableToMap( map, file.noteSectionTable(), file.fileHeader() );

    if( !file.symTab().isEmpty() ){
      setSymbolTableToMap(map, file.symTab(), file.fileHeader().ident);
    }
    setSymbolTableToMap(map, file.dynSym(), file.fileHeader().ident);

    setDynamicSectionToMap( map, file.dynamicSectionHeader(), file.dynamicSection(), file.fileHeader() );
    setStringTableToMap( map, file.headers().dynamicStringTableSectionHeader(), file.dynamicSection().stringTable() );
    setAllHeadersToMap( map, file.headers() );
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_H

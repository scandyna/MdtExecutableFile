// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_H
#define MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_H

#include "Mdt/ExecutableFile/Elf/GlobalOffsetTable.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <string>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  GlobalOffsetTableEntry globalOffsetTableEntryFromArray(const ByteArraySpan & array, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );
    assert( array.size == globalOffsetTableEntrySize(ident._class) );

    GlobalOffsetTableEntry entry;

    entry.data = getNWord(array.data, ident);

    return entry;
  }

  /*! \internal
   */
  inline
  GlobalOffsetTable extractGlobalOffsetTable(const ByteArraySpan & map, const FileHeader & fileHeader, const SectionHeader & sectionHeader) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( map.size >= sectionHeader.minimumSizeToReadSection() );
    assert( isGlobalOffsetTableSection(sectionHeader) );

    GlobalOffsetTable table;

    const int64_t entrySize = static_cast<int64_t>(sectionHeader.entsize);
    const int64_t offsetStart = static_cast<int64_t>(sectionHeader.offset);
    const int64_t offsetEnd = offsetStart + static_cast<int64_t>(sectionHeader.size);
    for(int64_t offset = offsetStart; offset < offsetEnd; offset += entrySize){
      const GlobalOffsetTableEntry entry = globalOffsetTableEntryFromArray(map.subSpan(offset, entrySize), fileHeader.ident);
      table.addEntryFromFile(entry);
    }

    return table;
  }

  /*! \internal
   *
   * If the section given by \a name does not exist, a empty table is returned
   */
  inline
  GlobalOffsetTable extractGlobalOffsetTable(const ByteArraySpan & map, const FileHeader & fileHeader,
                                             const std::vector<SectionHeader> & sectionHeaderTable,
                                             const std::string & sectionName) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( map.size >= fileHeader.minimumSizeToReadAllSectionHeaders() );

    const auto headerIt = findFirstSectionHeader(sectionHeaderTable, SectionType::ProgramData, sectionName);
    if( headerIt == sectionHeaderTable.cend() ){
      return GlobalOffsetTable();
    }
    assert( map.size >= headerIt->minimumSizeToReadSection() );

    return extractGlobalOffsetTable(map, fileHeader, *headerIt);
  }

  /*! \internal
   *
   * If the .got section does not exist, a empty table is returned
   */
  inline
  GlobalOffsetTable extractGotSection(const ByteArraySpan & map, const FileHeader & fileHeader,
                                      const std::vector<SectionHeader> & sectionHeaderTable) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( map.size >= fileHeader.minimumSizeToReadAllSectionHeaders() );

    return extractGlobalOffsetTable(map, fileHeader, sectionHeaderTable, ".got");
//     const auto headerIt = findFirstSectionHeader(sectionHeaderTable, SectionType::ProgramData, ".got");
//     if( headerIt == sectionHeaderTable.cend() ){
//       return GlobalOffsetTable();
//     }
//     assert( map.size >= headerIt->minimumSizeToReadSection() );
// 
//     return extractGlobalOffsetTable(map, fileHeader, *headerIt);
  }

  /*! \internal
   *
   * If the .got section does not exist, a empty table is returned
   */
  inline
  GlobalOffsetTable extractGotPltSection(const ByteArraySpan & map, const FileHeader & fileHeader,
                                         const std::vector<SectionHeader> & sectionHeaderTable) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( map.size >= fileHeader.minimumSizeToReadAllSectionHeaders() );

    return extractGlobalOffsetTable(map, fileHeader, sectionHeaderTable, ".got.plt");
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_H

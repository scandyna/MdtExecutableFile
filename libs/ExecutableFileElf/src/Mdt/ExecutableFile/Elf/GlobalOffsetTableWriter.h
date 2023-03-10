// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_WRITER_H

#include "Mdt/ExecutableFile/Elf/GlobalOffsetTable.h"
#include "Mdt/ExecutableFile/Elf/GlobalOffsetTableReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/Ident.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  void setGlobalOffsetTableEntryToArray(ByteArraySpan array, const GlobalOffsetTableEntry & entry, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );
    assert( array.size == globalOffsetTableEntrySize(ident._class) );

    setNWord(array, entry.data, ident);
  }

  /*! \internal
   */
  inline
  void setGlobalOffsetTableToMap(ByteArraySpan map, const SectionHeader & sectionHeader,
                                 const GlobalOffsetTable & table, const FileHeader & fileHeader) noexcept
  {
    assert( !map.isNull() );
    assert( !table.isEmpty() );
    assert( fileHeader.seemsValid() );
    assert( isGlobalOffsetTableSection(sectionHeader) );
    assert( map.size >= sectionHeader.minimumSizeToWriteSection() );

    const int64_t entrySize = globalOffsetTableEntrySize(fileHeader.ident._class);
    int64_t offset = static_cast<int64_t>(sectionHeader.offset);
    assert(offset >= 0);

    for(size_t i=0; i < table.entriesCount(); ++i){
      setGlobalOffsetTableEntryToArray( map.subSpan(offset, entrySize), table.entryAt(i), fileHeader.ident );
      offset += entrySize;
    }
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_WRITER_H

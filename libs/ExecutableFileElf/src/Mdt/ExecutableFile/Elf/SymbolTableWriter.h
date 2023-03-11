// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_SYMBOL_TABLE_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_SYMBOL_TABLE_WRITER_H

#include "Mdt/ExecutableFile/Elf/SymbolTable.h"
#include "Mdt/ExecutableFile/Elf/Ident.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cassert>

// #include <iostream>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  void setSymbolTableEntryToArray(ByteArraySpan array, const SymbolTableEntry & entry, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );
    assert( array.size == symbolTableEntrySize(ident._class) );

    set32BitWord(array.subSpan(0, 4), entry.name, ident.dataFormat);
    if(ident._class == Class::Class32){
      setNWord(array.subSpan(4, 4), entry.value, ident);
      setNWord(array.subSpan(8, 4), entry.size, ident);
      *(array.data + 12) = entry.info;
      *(array.data + 13) = entry.other;
      setHalfWord(array.subSpan(14, 2), entry.shndx, ident.dataFormat);
    }else{
      assert(ident._class == Class::Class64);
      *(array.data + 4) = entry.info;
      *(array.data + 5) = entry.other;
      setHalfWord(array.subSpan(6, 2), entry.shndx, ident.dataFormat);
      setNWord(array.subSpan(8, 8), entry.value, ident);
      setNWord(array.subSpan(16, 8), entry.size, ident);
    }
  }

  /*! \internal
   */
  inline
  void setSymbolTableToMap(ByteArraySpan map, const PartialSymbolTable & table, const Ident & ident) noexcept
  {
    assert( !map.isNull() );
    assert( ident.isValid() );
    assert( !table.isEmpty() );
    assert( map.size >= table.findMinimumSizeToAccessEntries(ident._class) );

    const int64_t entrySize = symbolTableEntrySize(ident._class);

    for(size_t i=0; i < table.entriesCount(); ++i){
      const int64_t fileOffset = table.fileMapOffsetAt(i);
      setSymbolTableEntryToArray( map.subSpan(fileOffset, entrySize), table.entryAt(i), ident );
    }
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_SYMBOL_TABLE_WRITER_H

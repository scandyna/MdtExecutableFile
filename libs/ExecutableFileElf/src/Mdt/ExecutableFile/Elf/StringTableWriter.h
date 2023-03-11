// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_STRING_TABLE_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_STRING_TABLE_WRITER_H

#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/StringTable.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <algorithm>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  void stringTableToArray(ByteArraySpan map, const StringTable & stringTable) noexcept
  {
    assert( !map.isNull() );
    assert( map.size >= stringTable.byteCount() );

    std::copy( stringTable.cbegin(), stringTable.cend(), map.data );
  }

  /*! \internal
   */
  inline
  int64_t minimumSizeToAccessStringTable(const SectionHeader & sectionHeader) noexcept
  {
    assert( sectionHeader.sectionType() == SectionType::StringTable );

    return static_cast<int64_t>(sectionHeader.offset + sectionHeader.size);
  }

  /*! \internal
   */
  inline
  bool mapIsBigEnoughToSetDynamicStringTable(const ByteArraySpan & map, const SectionHeader & sectionHeader) noexcept
  {
    assert( sectionHeader.sectionType() == SectionType::StringTable );

    return map.size >= minimumSizeToAccessStringTable(sectionHeader);
  }

  /*! \internal
   */
  inline
  void setStringTableToMap(ByteArraySpan map, const SectionHeader & sectionHeader, const StringTable & stringTable) noexcept
  {
    assert( !map.isNull() );
    assert( sectionHeader.sectionType() == SectionType::StringTable );
    assert( static_cast<int64_t>(sectionHeader.size) == stringTable.byteCount() );
    assert( mapIsBigEnoughToSetDynamicStringTable(map, sectionHeader) );
//     assert( map.size >= minimumSizeToAccessStringTable(sectionHeader) );

    const int64_t offset = static_cast<int64_t>(sectionHeader.offset);
    const int64_t size = static_cast<int64_t>(sectionHeader.size);

    stringTableToArray( map.subSpan(offset, size), stringTable );
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_STRING_TABLE_WRITER_H

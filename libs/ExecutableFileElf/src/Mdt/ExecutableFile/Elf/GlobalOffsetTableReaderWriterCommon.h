// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_WRITER_COMMON_H
#define MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_WRITER_COMMON_H

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  bool isGlobalOffsetTableSection(const SectionHeader & header) noexcept
  {
    assert( !header.name.empty() );

    if(header.sectionType() != SectionType::ProgramData){
      return false;
    }
    if(header.name == ".got"){
      return true;
    }
    if(header.name == ".got.plt"){
      return true;
    }

    return false;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_GLOBAL_OFFSET_TABLE_READER_WRITER_COMMON_H

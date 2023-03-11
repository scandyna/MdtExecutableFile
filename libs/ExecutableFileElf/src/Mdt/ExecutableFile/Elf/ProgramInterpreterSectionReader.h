// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_READER_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_READER_H

#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSection.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "Mdt/ExecutableFile/ExecutableFileReaderUtils.h"
#include <string>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  ProgramInterpreterSection extractProgramInterpreterSection(const ByteArraySpan & map, const SectionHeader & sectionHeader) noexcept
  {
    assert( !map.isNull() );
//     assert( fileHeader.seemsValid() );
    assert( map.size >= sectionHeader.minimumSizeToReadSection() );
    assert( sectionHeader.isProgramInterpreterSectionHeader() );

    ProgramInterpreterSection section;

    const int64_t offset = static_cast<int64_t>(sectionHeader.offset);
    const int64_t size = static_cast<int64_t>(sectionHeader.size);
    section.path = stringFromBoundedUnsignedCharArray( map.subSpan(offset, size) );

    return section;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_READER_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_WRITER_H

#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSection.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "Mdt/ExecutableFile/ExecutableFileReaderUtils.h"
#include <string>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  void setProgramInterpreterSectionToArray(ByteArraySpan array, const ProgramInterpreterSection & section) noexcept
  {
    assert( !array.isNull() );
    assert( array.size == static_cast<int64_t>( section.path.size() + 1 ) );

    setStringToUnsignedCharArray(array, section.path);
  }

  /*! \internal
   */
  inline
  void setProgramInterpreterSectionToMap(ByteArraySpan map, const SectionHeader & sectionHeader,
                                         const ProgramInterpreterSection & section) noexcept
  {
    assert( !map.isNull() );
//     assert( !table.isEmpty() );
    assert( sectionHeader.isProgramInterpreterSectionHeader() );
    assert( map.size >= sectionHeader.minimumSizeToWriteSection() );

    const int64_t offset = static_cast<int64_t>(sectionHeader.offset);
    const int64_t size = static_cast<int64_t>(sectionHeader.size);
    setProgramInterpreterSectionToArray(map.subSpan(offset, size), section);
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_WRITER_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_WRITER_COMMON_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_WRITER_COMMON_H

#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a fileHeader must be valid
   */
  inline
  bool programHeaderArraySizeIsBigEnough(const ByteArraySpan & array, const FileHeader & fileHeader) noexcept
  {
    assert( !array.isNull() );
    assert( fileHeader.seemsValid() );

    return array.size >= fileHeader.phentsize;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_WRITER_COMMON_H

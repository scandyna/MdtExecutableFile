// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_READER_WRITER_COMMON_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_READER_WRITER_COMMON_H

#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \pre \a array must not be null
   */
  inline
  bool identArraySizeIsBigEnough(const ByteArraySpan & array) noexcept
  {
    assert( !array.isNull() );

    return array.size >= 16;
  }

  /*! \internal
   *
   * \pre \a ident must be valid
   */
  inline
  int64_t minimumSizeToReadFileHeader(const Ident & ident) noexcept
  {
    assert( ident.isValid() );

    if( ident._class == Class::Class32 ){
      return 52;
    }
    assert( ident._class == Class::Class64 );

    return 64;
  }

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a ident must be valid
   */
  inline
  bool fileHeaderArraySizeIsBigEnough(const ByteArraySpan & array, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );

    return array.size >= minimumSizeToReadFileHeader(ident);
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_READER_WRITER_COMMON_H

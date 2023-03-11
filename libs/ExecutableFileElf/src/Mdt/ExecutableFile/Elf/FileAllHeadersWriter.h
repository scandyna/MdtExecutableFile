// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_WRITER_H

#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeadersReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileHeaderWriter.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderWriter.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderWriter.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  void setAllHeadersToMap(ByteArraySpan map, const FileAllHeaders & headers) noexcept
  {
    assert( !map.isNull() );
    assert( headers.seemsValid() );
    assert( map.size >= headers.minimumSizeToAccessAllHeaders() );

    fileHeaderToArray( map, headers.fileHeader() );
    setProgramHeadersToMap( map, headers.programHeaderTable(), headers.fileHeader() );
    setSectionHeadersToMap( map, headers.sectionHeaderTable(), headers.fileHeader() );
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_WRITER_H

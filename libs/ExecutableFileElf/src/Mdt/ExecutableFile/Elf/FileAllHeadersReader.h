// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_READER_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_READER_H

#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeadersReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderReader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"

#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  FileAllHeaders extractAllHeaders(const ByteArraySpan & map, const FileHeader & fileHeader)
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( map.size >= fileHeader.minimumSizeToReadAllProgramHeaders() );
    assert( map.size >= fileHeader.minimumSizeToReadAllSectionHeaders() );

    FileAllHeaders headers;

    headers.setFileHeader(fileHeader);
    headers.setProgramHeaderTable( extractAllProgramHeaders(map, fileHeader) );
    headers.setSectionHeaderTable( extractAllSectionHeaders(map, fileHeader) );

    return headers;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_ALL_HEADERS_READER_H

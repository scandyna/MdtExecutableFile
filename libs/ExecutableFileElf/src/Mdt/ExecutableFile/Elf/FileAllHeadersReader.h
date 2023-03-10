// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_READER_H
#define MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_READER_H

#include "FileAllHeaders.h"
#include "FileAllHeadersReaderWriterCommon.h"
#include "FileHeader.h"
#include "FileReader.h"
#include "ProgramHeaderReader.h"
#include "Mdt/DeployUtils/Impl/ByteArraySpan.h"
#include <cassert>

namespace Mdt{ namespace DeployUtils{ namespace Impl{ namespace Elf{

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

}}}} // namespace Mdt{ namespace DeployUtils{ namespace Impl{ namespace Elf{

#endif // #ifndef MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_READER_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_WRITER_H
#define MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_WRITER_H

#include "FileAllHeaders.h"
#include "FileAllHeadersReaderWriterCommon.h"
#include "FileHeader.h"
#include "FileHeaderWriter.h"
#include "ProgramHeaderWriter.h"
#include "SectionHeaderWriter.h"
#include "Mdt/DeployUtils/Impl/ByteArraySpan.h"
#include <cassert>

namespace Mdt{ namespace DeployUtils{ namespace Impl{ namespace Elf{

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

}}}} // namespace Mdt{ namespace DeployUtils{ namespace Impl{ namespace Elf{

#endif // #ifndef MDT_DEPLOY_UTILS_IMPL_ELF_FILE_ALL_HEADERS_WRITER_H

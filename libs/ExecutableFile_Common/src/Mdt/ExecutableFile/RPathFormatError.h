// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_RPATH_FORMAT_ERROR_H
#define MDT_EXECUTABLE_FILE_RPATH_FORMAT_ERROR_H

#include "Mdt/ExecutableFile/QRuntimeError.h"
#include "mdt_executablefile_common_export.h"
#include <QString>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Error when parsing a rpath string fails
   */
  class MDT_EXECUTABLEFILE_COMMON_EXPORT RPathFormatError : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit RPathFormatError(const QString & what)
      : QRuntimeError(what)
    {
    }

  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_RPATH_FORMAT_ERROR_H

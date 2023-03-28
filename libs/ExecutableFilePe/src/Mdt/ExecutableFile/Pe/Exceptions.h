// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PE_EXCEPTIONS_H
#define MDT_EXECUTABLE_FILE_PE_EXCEPTIONS_H

#include "Mdt/ExecutableFile/QRuntimeError.h"
// #include "mdt_deployutilscore_export.h"
#include <QString>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ InvalidMagicType : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit InvalidMagicType(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ FileCorrupted : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit FileCorrupted(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{

#endif // #ifndef MDT_EXECUTABLE_FILE_PE_EXCEPTIONS_H

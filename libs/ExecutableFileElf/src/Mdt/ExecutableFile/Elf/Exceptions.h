// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_EXCEPTIONS_H
#define MDT_EXECUTABLE_FILE_ELF_EXCEPTIONS_H

#include "Mdt/ExecutableFile/QRuntimeError.h"
// #include "mdt_deployutilscore_export.h"
#include <QString>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ DynamicSectionReadError : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit DynamicSectionReadError(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ NoteSectionReadError : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit NoteSectionReadError(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ MoveSectionError : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit MoveSectionError(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ GnuHashTableReadError : public QRuntimeError
  {
   public:

    /*! \brief Constructor
     */
    explicit GnuHashTableReadError(const QString & what)
      : QRuntimeError(what)
    {
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_EXCEPTIONS_H

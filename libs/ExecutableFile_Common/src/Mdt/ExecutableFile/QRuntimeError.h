// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2020-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_QRUNTIME_ERROR_H
#define MDT_EXECUTABLE_FILE_QRUNTIME_ERROR_H

#include "mdt_executablefile_common_export.h"
#include <QString>
#include <QByteArray>
#include <stdexcept>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Base class for runtime exceptions using QString messages
   */
  class MDT_EXECUTABLEFILE_COMMON_EXPORT QRuntimeError : public std::runtime_error
  {
   public:

    /*! \brief Constructor
     */
    explicit QRuntimeError(const QString & what)
     : runtime_error( what.toLocal8Bit().toStdString() )
    {
    }

    /*! \brief Get a QString version of what()
     */
    QString whatQString() const noexcept
    {
      return QString::fromLocal8Bit( QByteArray::fromStdString( what() ) );
    }
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_QRUNTIME_ERROR_H

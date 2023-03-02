// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/FileOpenError.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include "Mdt/ExecutableFile/ExecutableFileOpenMode.h"
#include "Mdt/ExecutableFile/Platform.h"
#include "mdt_executablefilecore_export.h"
#include <QObject>
#include <QFileInfo>
#include <memory>

namespace Mdt{ namespace ExecutableFile{

  class AbstractExecutableFileIoEngine;

  /*! \internal Helper class to instanciate a executable file I/O engine
   */
  class MDT_EXECUTABLEFILECORE_EXPORT ExecutableFileIoEngine : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit ExecutableFileIoEngine(QObject *parent = nullptr);

    /*! \brief Close this file engine and free resources
     */
    ~ExecutableFileIoEngine() noexcept;

   private:

    std::unique_ptr<AbstractExecutableFileIoEngine> mIoEngine;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H

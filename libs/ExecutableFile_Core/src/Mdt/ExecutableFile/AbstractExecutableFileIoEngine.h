// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/FileMapper.h"
#include "mdt_executablefilecore_export.h"
#include <QObject>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Interface to a minimal executable file I/O engine
   */
  class MDT_EXECUTABLEFILECORE_EXPORT AbstractExecutableFileIoEngine : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit AbstractExecutableFileIoEngine(QObject *parent = nullptr);

   private:

    FileMapper mFileMapper;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H

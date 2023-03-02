// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_FORMAT_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_FORMAT_H

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Executable file format
   */
  enum class ExecutableFileFormat
  {
    Elf,    /*!< Linux ELF executable */
    Pe,     /*!< Windows PE executable */
    Unknown /*!< Unknown format */
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_FORMAT_H

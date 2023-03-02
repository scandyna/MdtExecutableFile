// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_OPEN_MODE_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_OPEN_MODE_H

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Executable file open mode
   */
  enum class ExecutableFileOpenMode
  {
    ReadOnly,   /*!< Read only */
    ReadWrite   /*!< Read and write */
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_OPEN_MODE_H

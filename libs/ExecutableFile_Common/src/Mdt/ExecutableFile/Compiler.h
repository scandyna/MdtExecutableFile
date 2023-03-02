// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_COMPILER_H
#define MDT_EXECUTABLE_FILE_COMPILER_H

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Compiler enum
   */
  enum class Compiler
  {
    Unknown,  /*!< Unknown compiler */
    Gcc,      /*!< Gcc compiler */
    Clang,    /*!< Clang compiler */
    Msvc      /*!< MSVC compiler */
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_COMPILER_H

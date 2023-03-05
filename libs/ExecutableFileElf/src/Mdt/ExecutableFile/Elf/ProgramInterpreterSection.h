// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_H

#include <string>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  struct ProgramInterpreterSection
  {
    std::string path;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_INTERPRETER_SECTION_H

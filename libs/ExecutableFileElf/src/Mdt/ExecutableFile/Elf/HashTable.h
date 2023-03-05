// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_HASH_TABLE_H
#define MDT_EXECUTABLE_FILE_ELF_HASH_TABLE_H

#include <cstdint>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \sa https://flapenguin.me/elf-dt-hash
   * \sa https://refspecs.linuxfoundation.org/elf/gabi4+/ch5.dynamic.html#hash
   */
  struct HashTable
  {
    std::vector<uint32_t> bucket;
    std::vector<uint32_t> chain;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_HASH_TABLE_H

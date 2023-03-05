// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_H
#define MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_H

#include "Ident.h"
#include <cstdint>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \code
   * struct GnuHashTable
   * {
   *   uint32_t nbuckets;
   *   uint32_t symoffset;
   *   uint32_t bloomSize;
   *   uint32_t bloomShift;
   *   uint64_t bloom[bloom_size]; // uint32_t for 32-bit binaries
   *   uint32_t buckets[nbuckets];
   *   uint32_t chain[];
   * };
   * \endcode
   *
   * \sa https://flapenguin.me/elf-dt-gnu-hash
   */
  struct GnuHashTable
  {
    uint32_t symoffset;
    uint32_t bloomShift;
    std::vector<uint64_t> bloom;
    std::vector<uint32_t> buckets;
    std::vector<uint32_t> chain;

    /*! \brief get the count of buckets (nbuckets)
     */
    uint32_t bucketCount() const noexcept
    {
      return static_cast<uint32_t>( buckets.size() );
    }

    /*! \brief get the size of the bloom
     */
    uint32_t bloomSize() const noexcept
    {
      return static_cast<uint32_t>( bloom.size() );
    }

    /*! \brief Get the size, in bytes, of this hash table
     */
    int64_t byteCount(Class _class) const noexcept
    {
      assert(_class != Class::ClassNone);

      const int64_t bloomEntrySize = bloomEntryByteCount(_class);
      const int64_t bloomEntryCount = static_cast<int64_t>( bloom.size() );
      const int64_t bucketsEntryCount = static_cast<int64_t>( buckets.size() );
      const int64_t chainEntryCount = static_cast<int64_t>( chain.size() );

      return 16 + bloomEntrySize * bloomEntryCount + 4 * bucketsEntryCount + 4 * chainEntryCount;
    }

    /*! \brief Get the size (in bytes) for a bloom entry
     */
    static
    int64_t bloomEntryByteCount(Class _class) noexcept
    {
      if(_class == Class::Class64){
        return 8;
      }
      assert(_class == Class::Class32);

      return 4;
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_H

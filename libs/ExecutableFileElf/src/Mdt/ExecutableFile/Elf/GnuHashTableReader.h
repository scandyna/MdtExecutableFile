// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_READER_H
#define MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_READER_H

#include "Mdt/ExecutableFile/Elf/GnuHashTable.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/Exceptions.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <QObject>
#include <QString>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal Get the minimum size (in bytes) to read a GNU hash table
   *
   * We have to read at least nbuckets, symoffset, bloomSize and bloomShift.
   * Each one is a uint32_t (= 4 bytes each)
   * Total: 4x4=16 bytes
   */
  inline
  int64_t minimumSizeToReadGnuHashTable(uint64_t sectionSize) noexcept
  {
    assert(sectionSize >= 16);

    return static_cast<int64_t>(sectionSize);
  }

  /*! \internal
   */
  class GnuHashTableReader : public QObject
  {
    Q_OBJECT

   public:

    /*! \internal
     *
     * \note \a sectionSize is required to read the chain array
     *   (its size is not provided in the section itself)
     * \exception GnuHashTableReadError
     */
    static
    GnuHashTable hashTableFromArray(const ByteArraySpan & array, const Ident & ident, uint64_t sectionSize)
    {
      assert( !array.isNull() );
      assert( ident.isValid() );
      assert( array.size >= minimumSizeToReadGnuHashTable(sectionSize) );

      GnuHashTable table;

      const uint32_t bucketCount = getWord(array.subSpan(0, 4), ident.dataFormat);
      table.symoffset = getWord(array.subSpan(4, 4), ident.dataFormat);
      const uint32_t bloomSize = getWord(array.subSpan(8, 4), ident.dataFormat);
      table.bloomShift = getWord(array.subSpan(12, 4), ident.dataFormat);

      const int64_t bloomStartOffset = 16;
      const int64_t bloomEntrySize = GnuHashTable::bloomEntryByteCount(ident._class);
      const int64_t bloomEnd = bloomStartOffset + bloomEntrySize * static_cast<int64_t>(bloomSize);

      if(bloomEnd > array.size){
        const QString msg = tr("reading GNU hash table failed: bloom array ends past given array");
        throw GnuHashTableReadError(msg);
      }

      for(int64_t offset = bloomStartOffset; offset < bloomEnd; offset += bloomEntrySize){
        table.bloom.push_back( getNWord(array.subSpan(offset, bloomEntrySize), ident) );
      }

      const int64_t bucketsStartOffset = bloomEnd;
      const int64_t bucketEntrySize = 4;
      const int64_t bucketsEnd = bucketsStartOffset + bucketEntrySize * static_cast<int64_t>(bucketCount);

      if(bucketsEnd > array.size){
        const QString msg = tr("reading GNU hash table failed: buckets array ends past given array");
        throw GnuHashTableReadError(msg);
      }

      for(int64_t offset = bucketsStartOffset; offset < bucketsEnd; offset += bucketEntrySize){
        table.buckets.push_back( getWord(array.subSpan(offset, bucketEntrySize), ident.dataFormat) );
      }

      const int64_t chainStartOffset = bucketsEnd;
      const int64_t chainEntrySize = 4;
      const int64_t chainEnd = static_cast<int64_t>(sectionSize);

      for(int64_t offset = chainStartOffset; offset < chainEnd; offset += chainEntrySize){
        table.chain.push_back( getWord(array.subSpan(offset, chainEntrySize), ident.dataFormat) );
      }

      return table;
    }

    /*! \internal
     *
     * \exception GnuHashTableReadError
     */
    static
    GnuHashTable extractHasTable(const ByteArraySpan & map, const FileHeader & fileHeader, const SectionHeader & sectionHeader)
    {
      assert( !map.isNull() );
      assert( fileHeader.seemsValid() );
      assert( map.size >= sectionHeader.minimumSizeToReadSection() );
      assert( sectionHeader.isGnuHashTableSectionHeader() );

      const int64_t offset = static_cast<int64_t>(sectionHeader.offset);
      const int64_t size = static_cast<int64_t>(sectionHeader.size);

      try{
        return hashTableFromArray(map.subSpan(offset, size), fileHeader.ident, sectionHeader.size);
      }catch(const GnuHashTableReadError & error){
        const QString msg = tr("section %1 is corrupted: %2")
                            .arg( QString::fromStdString(sectionHeader.name), error.whatQString() );
        throw GnuHashTableReadError(msg);
      }
    }

  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_GNU_HASH_TABLE_READER_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_SECTION_INDEX_CHANGE_MAP_H
#define MDT_EXECUTABLE_FILE_ELF_SECTION_INDEX_CHANGE_MAP_H

#include <cstdint>
#include <vector>
#include <numeric>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class SectionIndexChangeMap
  {
   public:

    /*! \brief Construct a empty map
     */
    SectionIndexChangeMap() noexcept = default;

    /*! \brief Construct a map with \a indexCount indexes
     */
    SectionIndexChangeMap(uint16_t indexCount) noexcept
     : mMap(indexCount)
    {
      std::iota(mMap.begin(), mMap.end(), 0);
    }

    /*! \brief Copy construct a map from \a other
     */
    SectionIndexChangeMap(const SectionIndexChangeMap & other) = default;

    /*! \brief Copy assign \a other to this map
     */
    SectionIndexChangeMap & operator=(const SectionIndexChangeMap & other) = default;

    /*! \brief Move construct a map from \a other
     */
    SectionIndexChangeMap(SectionIndexChangeMap && other) noexcept = default;

    /*! \brief Move assign \a other to this map
     */
    SectionIndexChangeMap & operator=(SectionIndexChangeMap && other) noexcept = default;

    /*! \brief Swap 2 indexes
     *
     * The new index for the old one, \a a , will become \a b , and,
     * The new index for the old one, \a b , will become \a a .
     */
    void swapIndexes(uint16_t a, uint16_t b) noexcept
    {
      assert( a < mMap.size() );
      assert( b < mMap.size() );

      mMap[a] = b;
      mMap[b] = a;
    }

    /*! \brief Get the index for given \a oldIndex
     */
    uint16_t indexForOldIndex(uint16_t oldIndex) const noexcept
    {
      assert( oldIndex < mMap.size() );

      return mMap[oldIndex];
    }

    /*! \brief Get the count of indexes in this map
     */
    uint16_t indexCount() const noexcept
    {
      return static_cast<uint16_t>( mMap.size() );
    }

    /*! \brief Get the count of entries in this map
     */
    size_t entriesCount() const noexcept
    {
      return mMap.size();
    }

    /*! \brief Check if this map is empty
     */
    bool isEmpty() const noexcept
    {
      return mMap.empty();
    }

   private:

    std::vector<uint16_t> mMap;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_SECTION_INDEX_CHANGE_MAP_H

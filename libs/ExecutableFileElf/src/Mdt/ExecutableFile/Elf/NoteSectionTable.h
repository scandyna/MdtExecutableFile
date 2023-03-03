// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_TABLE_H
#define MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_TABLE_H

#include "Mdt/ExecutableFile/Elf/NoteSection.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  struct NoteSectionTableEntry
  {
    SectionHeader header;
    NoteSection section;

    NoteSectionTableEntry() = delete;

    NoteSectionTableEntry(const SectionHeader & _header, const NoteSection & _section) noexcept
     : header(_header),
       section(_section)
    {
    }

    NoteSectionTableEntry(const NoteSectionTableEntry & other) = default;
    NoteSectionTableEntry & operator=(const NoteSectionTableEntry & other) = default;
    NoteSectionTableEntry(NoteSectionTableEntry && other) noexcept = default;
    NoteSectionTableEntry & operator=(NoteSectionTableEntry && other) noexcept = default;
  };

  /*! \internal
   */
  class NoteSectionTable
  {
   public:

    /*! \brief Add a section to this table
     *
     * \pre \a header must be a note section header
     */
    void addSectionFromFile(const SectionHeader & header, const NoteSection & section) noexcept
    {
      assert( isNoteSectionHeader(header) );

      mTable.emplace_back(header, section);
    }

    /*! \brief Get the count of sections in this table
     */
    size_t sectionCount() const noexcept
    {
      return mTable.size();
    }

    /*! \brief Get the section name at \a index
     *
     * \pre \a index must be in a valid range
     */
    const std::string & sectionNameAt(size_t index) const noexcept
    {
      assert( index < sectionCount() );

      return mTable[index].header.name;
    }

    /*! \brief Get the section header at \a index
     *
     * \pre \a index must be in a valid range
     */
    const SectionHeader & sectionHeaderAt(size_t index) const noexcept
    {
      assert( index < sectionCount() );

      return mTable[index].header;
    }

    /*! \brief Get the section at \a index
     *
     * \pre \a index must be in a valid range
     */
    const NoteSection & sectionAt(size_t index) const noexcept
    {
      assert( index < sectionCount() );

      return mTable[index].section;
    }

    /*! \brief Update the section headers regarding \a sectionHeaderTable
     *
     * Should be called if some note sections are moved
     * in the section header table, so that the section headers in this table
     * are up to date.
     */
    void updateSectionHeaders(const std::vector<SectionHeader> & sectionHeaderTable) noexcept
    {
      for(const SectionHeader & shtHeader : sectionHeaderTable){
        if( isNoteSectionHeader(shtHeader) ){
          const auto it = findSectionHeader(shtHeader.name);
          if( it != mTable.cend() ){
            it->header = shtHeader;
          }
        }
      }
    }

    /*! \brief Find the minimum size required to write this table to a file
     */
    int64_t findMinimumSizeToWriteTable() const noexcept
    {
      const auto cmp = [](const NoteSectionTableEntry & a, const NoteSectionTableEntry & b){
        return a.header.minimumSizeToWriteSection() < b.header.minimumSizeToWriteSection();
      };

      const auto it = std::max_element(mTable.cbegin(), mTable.cend(), cmp);
      if( it == mTable.cend() ){
        return 0;
      }

      return it->header.minimumSizeToWriteSection();
    }

   private:

    using iterator = std::vector<NoteSectionTableEntry>::iterator;

    iterator findSectionHeader(const std::string & name) noexcept
    {
      const auto pred = [&name](const NoteSectionTableEntry & entry){
        return entry.header.name == name;
      };

      return std::find_if(mTable.begin(), mTable.end(), pred);
    }

    std::vector<NoteSectionTableEntry> mTable;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_TABLE_H

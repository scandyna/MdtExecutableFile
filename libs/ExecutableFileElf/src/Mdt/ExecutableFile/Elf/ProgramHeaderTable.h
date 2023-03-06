// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_TABLE_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_TABLE_H

#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <cassert>

// #include <iostream>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal Represents the program header table in a ELF file
   */
  class ProgramHeaderTable
  {
   public:

    /*! \brief STL const iterator
     */
    using const_iterator = std::vector<ProgramHeader>::const_iterator;

    /*! \brief STL iterator
     */
    using iterator = std::vector<ProgramHeader>::iterator;

    /*! \brief Get the count of headers in this table
     */
    size_t headerCount() const noexcept
    {
      return mTable.size();
    }

    /*! \brief Check if this table is empty
     */
    bool isEmpty() const noexcept
    {
      return mTable.empty();
    }

    /*! \brief Get the program header at \a index
     *
     * \pre \a index must be in valid range ( \a index < headerCount() )
     */
    const ProgramHeader & headerAt(size_t index) const noexcept
    {
      assert( index < headerCount() );

      return mTable[index];
    }

    /*! \brief Add \a header readen from a file
     *
     * This method simply adds \a header,
     * id does not adjust anything in this table
     * (except setting the internal index of a known segment).
     */
    void addHeaderFromFile(const ProgramHeader & header) noexcept
    {
      switch( header.segmentType() ){
        case SegmentType::Dynamic:
          mDynamicSectionHeaderIndex = mTable.size();
          break;
        case SegmentType::ProgramHeaderTable:
          mProgramHeaderTableHeaderIndex = mTable.size();
          break;
        case SegmentType::Interpreter:
          mProgramInterpreterHeaderIndex = mTable.size();
          break;
        case SegmentType::Note:
          mNoteSegmentHeaderIndex = mTable.size();
          break;
        case SegmentType::GnuRelRo:
          mGnuRelRoSegmentHeaderIndex = mTable.size();
          break;
        default:
          break;
      }

      mTable.push_back(header);
    }

    /*! \brief Add \a header to this table
     *
     * \sa addHeaderFromFile()
     */
    void addHeader(const ProgramHeader & header, uint16_t programHeaderEntrySize) noexcept
    {
      addHeaderFromFile(header);

      if( containsProgramHeaderTableHeader() ){
        const uint64_t size = mTable.size() * programHeaderEntrySize;
        mTable[mProgramHeaderTableHeaderIndex].memsz = size;
        mTable[mProgramHeaderTableHeaderIndex].filesz = size;
      }
    }

    /*! \brief Add a new load segment to the end of the program header table
     */
    [[deprecated]]
    ProgramHeader & appendNullLoadSegment(uint16_t programHeaderEntrySize) noexcept
    {
      ProgramHeader header;

      header.setSegmentType(SegmentType::Load);
      header.flags = 0;
      header.offset = 0;
      header.vaddr = 0;
      header.paddr = 0;
      header.filesz = 0;
      header.memsz = 0;
      header.align = 0;

      addHeader(header, programHeaderEntrySize);

      return mTable[mTable.size()-1];
    }

    /*! \brief Check if the program header for the program header table exists (PT_PHDR)
     */
    bool containsProgramHeaderTableHeader() const noexcept
    {
      return mProgramHeaderTableHeaderIndex < std::numeric_limits<size_t>::max();
    }

    /*! \brief Get the program header table program header (PT_PHDR)
     *
     * \pre the program header table program header must exist
     * \sa containsProgramHeaderTableProgramHeader()
     */
    const ProgramHeader & programHeaderTableHeader() const noexcept
    {
      assert( containsProgramHeaderTableHeader() );

      return mTable[mProgramHeaderTableHeaderIndex];
    }

    /*! \brief Set the virtual address and offset of the program header table program header (PT_PHDR)
     */
    void setProgramHeaderTableHeaderVirtualAddressAndFileOffset(uint64_t virtualAddress, uint64_t fileOffset) noexcept
    {
      assert( containsProgramHeaderTableHeader() );

      mTable[mProgramHeaderTableHeaderIndex].vaddr = virtualAddress;
      mTable[mProgramHeaderTableHeaderIndex].paddr = virtualAddress;
      mTable[mProgramHeaderTableHeaderIndex].offset = fileOffset;
    }

    /*! \brief Check if this table contains the program header of the dynamic section
     */
    bool containsDynamicSectionHeader() const noexcept
    {
      return mDynamicSectionHeaderIndex < std::numeric_limits<size_t>::max();
    }

    /*! \brief Get the program header of the dynamic section
     *
     * \pre the program header of the dynamic section must exist in this table
     */
    const ProgramHeader & dynamicSectionHeader() const noexcept
    {
      assert( containsDynamicSectionHeader() );

      return mTable[mDynamicSectionHeaderIndex];
    }

    /*! \brief Set the size of the dynamic section
     *
     * \pre the program header of the dynamic section must exist in this table
     * \sa containsDynamicSectionHeader()
     */
    void setDynamicSectionSize(uint64_t size) noexcept
    {
      assert( containsDynamicSectionHeader() );

      mTable[mDynamicSectionHeaderIndex].memsz = size;
      mTable[mDynamicSectionHeaderIndex].filesz = size;
    }

    /*! \brief Set the virtual address and file offset of the dynamic section
     *
     * \pre the program header of the dynamic section must exist in this table
     * \sa containsDynamicSectionHeader()
     */
    void setDynamicSectionVirtualAddressAndFileOffset(uint64_t virtualAddress, uint64_t fileOffset) noexcept
    {
      assert( containsDynamicSectionHeader() );

      mTable[mDynamicSectionHeaderIndex].vaddr = virtualAddress;
      mTable[mDynamicSectionHeaderIndex].paddr = virtualAddress;
      mTable[mDynamicSectionHeaderIndex].offset = fileOffset;
    }

    /*! \brief Check if the .interp program header exists
     */
    bool containsProgramInterpreterProgramHeader() const noexcept
    {
      return mProgramInterpreterHeaderIndex < mTable.size();
    }

    /*! \brief Get the .interp program header
     *
     * \pre the .interp program header must exist
     * \sa containsProgramInterpreterProgramHeader()
     */
    const ProgramHeader & programInterpreterProgramHeader() const noexcept
    {
      assert( containsProgramInterpreterProgramHeader() );

      return mTable[mProgramInterpreterHeaderIndex];
    }

    /*! \brief Set the virtual address and offset of the program interpreter header (PT_INTERP)
     */
    void setProgramInterpreterHeaderVirtualAddressAndFileOffset(uint64_t virtualAddress, uint64_t fileOffset) noexcept
    {
      assert( containsProgramInterpreterProgramHeader() );

      mTable[mProgramInterpreterHeaderIndex].vaddr = virtualAddress;
      mTable[mProgramInterpreterHeaderIndex].paddr = virtualAddress;
      mTable[mProgramInterpreterHeaderIndex].offset = fileOffset;
    }

    /*! \brief Check if the PT_NOTE program header exists
     */
    bool containsNoteProgramHeader() const noexcept
    {
      return mNoteSegmentHeaderIndex < mTable.size();
    }

    /*! \brief Get the PT_NOTE program header
     *
     * \pre the PT_NOTE program header must exist
     * \sa containsNoteProgramHeader()
     */
    const ProgramHeader & noteProgramHeader() const noexcept
    {
      assert( containsNoteProgramHeader() );

      return mTable[mNoteSegmentHeaderIndex];
    }

    /*! \brief Set the virtual address and offset of the note header (PT_NOTE)
     */
    void setNoteProgramHeaderVirtualAddressAndFileOffset(uint64_t virtualAddress, uint64_t fileOffset) noexcept
    {
      assert( containsNoteProgramHeader() );

      mTable[mNoteSegmentHeaderIndex].vaddr = virtualAddress;
      mTable[mNoteSegmentHeaderIndex].paddr = virtualAddress;
      mTable[mNoteSegmentHeaderIndex].offset = fileOffset;
    }

    /*! \brief Check if this table contains the PT_GNU_RELRO header
     */
    bool containsGnuRelRoHeader() const noexcept
    {
      return mGnuRelRoSegmentHeaderIndex < mTable.size();
    }

    /*! \brief Get the PT_GNU_RELRO program header
     *
     * \pre the PT_GNU_RELRO program header must exist
     * \sa containsGnuRelRoHeader()
     */
    const ProgramHeader & gnuRelRoHeader() const noexcept
    {
      assert( containsGnuRelRoHeader() );

      return mTable[mGnuRelRoSegmentHeaderIndex];
    }

    /*! \brief Get the PT_GNU_RELRO program header
     *
     * \pre the PT_GNU_RELRO program header must exist
     * \sa containsGnuRelRoHeader()
     */
    ProgramHeader & gnuRelRoHeaderMutable() noexcept
    {
      assert( containsGnuRelRoHeader() );

      return mTable[mGnuRelRoSegmentHeaderIndex];
    }

    /*! \brief Set the size for the PT_GNU_RELRO header
     *
     * \pre the PT_GNU_RELRO program header must exist
     * \sa containsGnuRelRoHeader()
     */
    void setGnuRelRoHeaderSize(uint64_t size) noexcept
    {
      assert( containsGnuRelRoHeader() );

      mTable[mGnuRelRoSegmentHeaderIndex].memsz = size;
      mTable[mGnuRelRoSegmentHeaderIndex].filesz = size;
    }

    /*! \brief Get the virtual address of the end of the last segment of this table
     *
     * \note the returned address is 1 byte past the last virtual address of the last segment
     * \pre this table must not be empty
     */
    uint64_t findLastSegmentVirtualAddressEnd() const noexcept
    {
      assert( !isEmpty() );

      const auto cmp = [](const ProgramHeader & a, const ProgramHeader & b){
        return a.segmentVirtualAddressEnd() < b.segmentVirtualAddressEnd();
      };

      const auto it = std::max_element(cbegin(), cend(), cmp);
      assert( it != cend() );

      return it->segmentVirtualAddressEnd();
    }

    /*! \brief Get the file offset of the last segment of this table
     *
     * \note the returned offset is 1 byte past the last offset of the last segment
     * \pre this table must not be empty
     */
    uint64_t findLastSegmentFileOffsetEnd() const noexcept
    {
      assert( !isEmpty() );

      const auto cmp = [](const ProgramHeader & a, const ProgramHeader & b){
        return a.fileOffsetEnd() < b.fileOffsetEnd();
      };

      const auto it = std::max_element(cbegin(), cend(), cmp);
      assert( it != cend() );

      return it->fileOffsetEnd();
    }

    /*! \brief get the begin iterator
     */
    const_iterator cbegin() const noexcept
    {
      return mTable.cbegin();
    }

    /*! \brief get the begin iterator
     */
    const_iterator begin() const noexcept
    {
      return mTable.cbegin();
    }

    /*! \brief get the begin iterator
     */
    iterator begin() noexcept
    {
      return mTable.begin();
    }

    /*! \brief get the begin iterator
     */
    const_iterator cend() const noexcept
    {
      return mTable.cend();
    }

    /*! \brief get the begin iterator
     */
    const_iterator end() const noexcept
    {
      return mTable.cend();
    }

    /*! \brief get the begin iterator
     */
    iterator end() noexcept
    {
      return mTable.end();
    }

   private:

    static
    constexpr size_t invalidHeaderIndex() noexcept
    {
      return std::numeric_limits<size_t>::max();
    }

    size_t mDynamicSectionHeaderIndex = invalidHeaderIndex();
    size_t mProgramHeaderTableHeaderIndex = invalidHeaderIndex();
    size_t mProgramInterpreterHeaderIndex = invalidHeaderIndex();
    size_t mNoteSegmentHeaderIndex = invalidHeaderIndex();
    size_t mGnuRelRoSegmentHeaderIndex = invalidHeaderIndex();
    std::vector<ProgramHeader> mTable;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_TABLE_H

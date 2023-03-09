// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_FILE_LAYOUT_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_FILE_LAYOUT_H

#include "Mdt/ExecutableFile/Elf/OffsetRange.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class FileWriterFileLayout
  {
  public:

    OffsetRange dynamicSectionOffsetRange() const noexcept
    {
      return mDynamicSectionOffsetRange;
    }

    uint64_t dynamicSectionOffset() const noexcept
    {
      return mDynamicSectionOffsetRange.begin();
    }

    uint64_t dynamicSectionSize() const noexcept
    {
      return mDynamicSectionOffsetRange.byteCount();
    }

    OffsetRange dynamicStringTableOffsetRange() const noexcept
    {
      return mDynamicStringTableOffsetRange;
    }

    uint64_t dynamicStringTableOffset() const noexcept
    {
      return mDynamicStringTableOffsetRange.begin();
    }

    uint64_t dynamicStringTableSize() const noexcept
    {
      return mDynamicStringTableOffsetRange.byteCount();
    }

    OffsetRange globalOffsetRange() const noexcept
    {
      return mGlobalOffsetRange;
    }

    /*! \brief Get a file layout from a file
     *
     * \pre \a headers must be valid
     * \pre \a headers must contain the dynamic program header
     * \pre \a headers must contain the dynamic section header
     */
    static
    FileWriterFileLayout fromFile(const FileAllHeaders & headers) noexcept
    {
      assert( headers.seemsValid() );
      assert( headers.containsDynamicProgramHeader() );
      assert( headers.containsDynamicSectionHeader() );
      assert( headers.containsDynamicStringTableSectionHeader() );

      FileWriterFileLayout layout;
      layout.mDynamicSectionOffsetRange = OffsetRange::fromProgrameHeader( headers.dynamicProgramHeader() );
      layout.mDynamicStringTableOffsetRange = OffsetRange::fromSectionHeader( headers.dynamicStringTableSectionHeader() );
      layout.mGlobalOffsetRange = headers.globalFileOffsetRange();

      return layout;
    }

  private:

    OffsetRange mDynamicSectionOffsetRange;
    OffsetRange mDynamicStringTableOffsetRange;
    OffsetRange mGlobalOffsetRange;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_WRITER_FILE_LAYOUT_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_H
#define MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_H

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/Algorithm.h"
#include <cstdint>
#include <string>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \sa https://www.netbsd.org/docs/kernel/elf-notes.html
   *
   * For a list of some existing note section,
   * see https://man7.org/linux/man-pages/man5/elf.5.html
   */
  struct NoteSection
  {
    //uint32_t nameSize;
    uint32_t descriptionSize; // [byte]
    uint32_t type;
    std::string name;
    std::vector<uint32_t> description;

    /*! \brief Check if this section is null
     */
    bool isNull() const noexcept
    {
      return name.empty();
    }

    /*! \brief Get the size of this note section
     *
     * \pre this section must not be null
     */
    int64_t byteCountAligned() const noexcept
    {
      assert( !isNull() );

      /*
       * name size: 4 bytes
       * description size: 4 bytes
       * type: 4 bytes
       * name: name size aligned to 4 bytes (must include the null termiation char)
       * description: description size aligned to 4 bytes
       */
      const int64_t nameSize = static_cast<int64_t>( findAlignedSize(static_cast<uint64_t>(name.size()+1), 4) );
      const int64_t descriptionSize = static_cast<int64_t>(description.size() * 4);
      assert( nameSize >= 0 );
      assert( descriptionSize >= 0 );

      return 4 + 4 + 4 + nameSize + descriptionSize;
    }

    /*! \brief Get the minimum size of a note section
     */
    static
    int64_t minimumByteBount() noexcept
    {
      return 4 + 4 + 4;
    }

    /*! \brief Get the maximum size of the name section
     */
    static
    int64_t maximumNameSize(int64_t sectionSize) noexcept
    {
      assert( sectionSize > 0 );

      int64_t result = 0;

      if( sectionSize < minimumByteBount() ){
        return 0;
      }

      result = sectionSize - minimumByteBount();
      assert( result >= 0 );

      return result;
    }
  };

  /*! \internal
   */
  inline
  bool isNoteSectionHeader(const SectionHeader & header) noexcept
  {
    return header.sectionType() == SectionType::Note;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_H

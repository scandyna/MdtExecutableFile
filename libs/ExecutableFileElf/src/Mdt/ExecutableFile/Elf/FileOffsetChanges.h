// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_OFFSET_CHANGES_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_OFFSET_CHANGES_H

#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include "Mdt/ExecutableFile/Elf/StringTable.h"
#include "Mdt/ExecutableFile/Elf/Ident.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class FileOffsetChanges
  {
   public:

    /*! \brief Set the original sizes regarding \a dynamicSection and its related string table
     *
     * \pre the given class must not be \a ClassNone
     */
    void setOriginalSizes(const DynamicSection & dynamicSection, Class c) noexcept
    {
      assert(c != Class::ClassNone);

      mOriginalDynamicSectionByteCount = dynamicSection.byteCount(c);
      mOriginalDynamicStringTableByteCount = dynamicSection.stringTable().byteCount();
      mClass = c;
    }

    /*! \brief Get the offset of the changes applyed to the dynamic section (in bytes)
     *
     * For example, if a entry was added since the call to setOriginalSizes(),
     * 16 will be returned for a 64-bit file.
     *
     * If a entry was removed, -16 will be returned for a 64-bit file.
     */
    int64_t dynamicSectionChangesOffset(const DynamicSection & dynamicSection) const noexcept
    {
      assert(mClass != Class::ClassNone);

      return dynamicSection.byteCount(mClass) - mOriginalDynamicSectionByteCount;
    }

    /*! \brief Get the offset of changes applyed to the dynamic string table related to \a dynamicSection (in bytes)
     */
    int64_t dynamicStringTableChangesOffset(const DynamicSection & dynamicSection) const noexcept
    {
      assert(mClass != Class::ClassNone);

      return dynamicSection.stringTable().byteCount() - mOriginalDynamicStringTableByteCount;
    }

    /*! \brief Get the offset of changes applyed to the dynamic section and its related string table
     */
    int64_t globalChangesOffset(const DynamicSection & dynamicSection) const noexcept
    {
      assert(mClass != Class::ClassNone);

      return dynamicSectionChangesOffset(dynamicSection) + dynamicStringTableChangesOffset(dynamicSection);
    }

   private:

    int64_t mOriginalDynamicSectionByteCount = 0;
    int64_t mOriginalDynamicStringTableByteCount = 0;
    Class mClass = Class::ClassNone;
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_OFFSET_CHANGES_H

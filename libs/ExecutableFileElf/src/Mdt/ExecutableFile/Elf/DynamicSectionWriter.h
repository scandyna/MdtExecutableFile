// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_DYNAMIC_SECTION_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_DYNAMIC_SECTION_WRITER_H

#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"

// #include "Debug.h"
// #include <iostream>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  inline
  int64_t dynamicSectionByteCount(const DynamicSection & dynamicSection, const Ident & ident) noexcept
  {
    assert( ident.isValid() );

    return dynamicSection.byteCount(ident._class);
  }

  /*! \internal
   */
  inline
  bool dynamicSectionArraySizeIsBigEnough(const ByteArraySpan & array, const DynamicSection & dynamicSection, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );

    return array.size >= dynamicSectionByteCount(dynamicSection, ident);
  }

  /*! \internal
   */
  inline
  void dynamicSectionToArray(ByteArraySpan array, const DynamicSection & dynamicSection, const Ident & ident) noexcept
  {
    assert( !array.isNull() );
    assert( ident.isValid() );
    assert( dynamicSectionArraySizeIsBigEnough(array, dynamicSection, ident) );

    int64_t offset = 0;

    if(ident._class == Class::Class32){
      for(const DynamicStruct & entry : dynamicSection){
        setSignedNWord( array.subSpan(offset, 4), entry.tag, ident );
        offset += 4;
        setNWord( array.subSpan(offset, 4), entry.val_or_ptr, ident );
        offset += 4;
      }
    }else{
      assert(ident._class == Class::Class64);
      for(const DynamicStruct & entry : dynamicSection){
        setSignedNWord( array.subSpan(offset, 8), entry.tag, ident );
        offset += 8;
        setNWord( array.subSpan(offset, 8), entry.val_or_ptr, ident );
        offset += 8;
      }
    }
  }

  /*! \internal
   */
  inline
  bool dynamicSectionHeaderSizeMatchesDynamicSectionSize(const SectionHeader & dynamicSectionHeader, const DynamicSection & dynamicSection, const Ident & ident) noexcept
  {
    assert( dynamicSectionHeader.sectionType() == SectionType::Dynamic );
    assert( ident.isValid() );

    return static_cast<int64_t>(dynamicSectionHeader.size) == dynamicSectionByteCount(dynamicSection, ident);
  }

  /*! \internal
   */
  inline
  int64_t minimumSizeToAccessDynamicSection(const SectionHeader & dynamicSectionHeader) noexcept
  {
    assert( dynamicSectionHeader.sectionType() == SectionType::Dynamic );

    return static_cast<int64_t>(dynamicSectionHeader.offset + dynamicSectionHeader.size);
  }

  /*! \internal
   */
  inline
  bool mapIsBigEnoughToSetDynamicSection(const ByteArraySpan & map, const SectionHeader & dynamicSectionHeader) noexcept
  {
    assert( dynamicSectionHeader.sectionType() == SectionType::Dynamic );

    return map.size >= minimumSizeToAccessDynamicSection(dynamicSectionHeader);
  }

  /*! \internal Set the dynamic section to \a map
   *
   * \note this function will not set any header related to \a dynamicSection to \a map
   */
  inline
  void setDynamicSectionToMap(ByteArraySpan map, const SectionHeader & dynamicSectionHeader,
                              const DynamicSection & dynamicSection, const FileHeader & fileHeader) noexcept
  {
    assert( !map.isNull() );
    assert( dynamicSectionHeader.sectionType() == SectionType::Dynamic );
    assert( fileHeader.seemsValid() );
    assert( dynamicSectionHeaderSizeMatchesDynamicSectionSize(dynamicSectionHeader, dynamicSection, fileHeader.ident) );
    assert( mapIsBigEnoughToSetDynamicSection(map, dynamicSectionHeader) );
//     assert( map.size >= minimumSizeToAccessDynamicSection(dynamicSectionHeader) );
//     assert( sectionNamesStringTableSectionHeader.sectionType() == SectionType::StringTable );

    const int64_t offset = static_cast<int64_t>(dynamicSectionHeader.offset);
    const int64_t size = static_cast<int64_t>(dynamicSectionHeader.size);

    dynamicSectionToArray( map.subSpan(offset, size), dynamicSection, fileHeader.ident );
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_DYNAMIC_SECTION_WRITER_H

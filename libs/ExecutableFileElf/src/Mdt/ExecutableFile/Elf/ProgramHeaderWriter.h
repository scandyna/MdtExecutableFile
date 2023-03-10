// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_WRITER_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_WRITER_H

#include "Mdt/ExecutableFile/Elf/ProgramHeaderReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileWriterUtils.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array must be big enough to hold a program header
   * 
   * \todo some preconditions on \a programHeader ?
   * 
   * \pre \a fileHeader must be valid
   * \sa programHeaderArraySizeIsBigEnough()
   */
  inline
  void programHeaderToArray(ByteArraySpan array, const ProgramHeader & programHeader, const FileHeader & fileHeader) noexcept
  {
    assert( !array.isNull() );
    assert( fileHeader.seemsValid() );
    
    assert( programHeaderArraySizeIsBigEnough(array, fileHeader) );

    const Ident ident = fileHeader.ident;

    set32BitWord(array.subSpan(0, 4), programHeader.type, ident.dataFormat);

    if(ident._class == Class::Class32){
      setOffset(array.subSpan(0x04, 4), programHeader.offset, ident);
      setAddress(array.subSpan(0x08, 4), programHeader.vaddr, ident);
      setAddress(array.subSpan(0x0C, 4), programHeader.paddr, ident);
      setNWord(array.subSpan(0x10, 4), programHeader.filesz, ident);
      setNWord(array.subSpan(0x14, 4), programHeader.memsz, ident);
      setNWord(array.subSpan(0x18, 4), programHeader.flags, ident);
      setNWord(array.subSpan(0x1C, 4), programHeader.align, ident);
    }else{
      assert( ident._class == Class::Class64 );
      set32BitWord(array.subSpan(0x04, 4), programHeader.flags, ident.dataFormat);
      setOffset(array.subSpan(0x08, 8), programHeader.offset, ident);
      setAddress(array.subSpan(0x10, 8), programHeader.vaddr, ident);
      setAddress(array.subSpan(0x18, 8), programHeader.paddr, ident);
      setNWord(array.subSpan(0x20, 8), programHeader.filesz, ident);
      setNWord(array.subSpan(0x28, 8), programHeader.memsz, ident);
      setNWord(array.subSpan(0x30, 8), programHeader.align, ident);
    }
  }

  /*! \internal Check that the count of headers matches the one set in \a fileHeader
   */
  inline
  bool fileHeaderMatchesProgramHeadersCounts(const FileHeader & fileHeader, const ProgramHeaderTable & programHeaders) noexcept
  {
    assert( fileHeader.seemsValid() );

    return fileHeader.phnum == programHeaders.headerCount();
  }

  /*! \internal
   */
  inline
  void setProgramHeadersToMap(ByteArraySpan map, const ProgramHeaderTable & programHeaders, const FileHeader & fileHeader) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( fileHeaderMatchesProgramHeadersCounts(fileHeader, programHeaders) );
    assert( map.size >= fileHeader.minimumSizeToReadAllProgramHeaders() );

    const uint16_t programHeaderCount = fileHeader.phnum;
    const int64_t start = static_cast<int64_t>(fileHeader.phoff);

    for(uint16_t i = 0; i < programHeaderCount; ++i){
      const int64_t offset = start + i * fileHeader.phentsize;
      const int64_t size = static_cast<int64_t>(fileHeader.phentsize);
      programHeaderToArray(map.subSpan(offset, size), programHeaders.headerAt(i), fileHeader);
    }
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_WRITER_H

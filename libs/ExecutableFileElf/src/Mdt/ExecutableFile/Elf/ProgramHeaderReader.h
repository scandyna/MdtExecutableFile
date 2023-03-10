// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_H

#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderReaderWriterCommon.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/Exceptions.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array must be big enough to hold a program header
   * \pre \a fileHeader must be valid
   * \sa programHeaderArraySizeIsBigEnough()
   */
  inline
  ProgramHeader programHeaderFromArray(const ByteArraySpan & array, const FileHeader & fileHeader) noexcept
  {
    assert( !array.isNull() );
    assert( fileHeader.seemsValid() );
    assert( programHeaderArraySizeIsBigEnough(array, fileHeader) );

    ProgramHeader programHeader;
    const Ident ident = fileHeader.ident;
    const unsigned char *it = array.data;

    programHeader.type = getWord(it, ident.dataFormat);
    it += 4;

    if(ident._class == Class::Class32){
      programHeader.offset = getOffset(it, ident);
      it += 4;
      programHeader.vaddr = getAddress(it, ident);
      it += 4;
      programHeader.paddr = getAddress(it, ident);
      it += 4;
      programHeader.filesz = getWord(it, ident.dataFormat);
      it += 4;
      programHeader.memsz = getWord(it, ident.dataFormat);
      it += 4;
      programHeader.flags = getWord(it, ident.dataFormat);
      it += 4;
      programHeader.align = getWord(it, ident.dataFormat);
    }else{
      assert( ident._class == Class::Class64 );
      programHeader.flags = getWord(it, ident.dataFormat);
      it += 4;
      programHeader.offset = getOffset(it, ident);
      it += 8;
      programHeader.vaddr = getAddress(it, ident);
      it += 8;
      programHeader.paddr = getAddress(it, ident);
      it += 8;
      programHeader.filesz = getWord(it, ident.dataFormat);
      it += 8;
      programHeader.memsz = getWord(it, ident.dataFormat);
      it += 8;
      programHeader.align = getWord(it, ident.dataFormat);
    }

    return programHeader;
  }

  /*! \internal Get the minimum size (in bytes) required to extract the program header at \a index
   *
   * \pre \a fileHeader must be valid
   */
  inline
  int64_t minimumSizeToExtractProgramHeaderAt(const FileHeader & fileHeader, uint16_t index) noexcept
  {
    assert( fileHeader.seemsValid() );

    return static_cast<int64_t>(fileHeader.phoff) + index * fileHeader.phentsize;
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map must be big enough to extract the program header at \a index
   * \pre \a fileHeader must be valid
   * \pre \a index must be < file header's program headers count
   * \sa minimumSizeToExtractProgramHeaderAt()
   */
  inline
  ProgramHeader extractProgramHeaderAt(const ByteArraySpan & map, const FileHeader & fileHeader, uint16_t index) noexcept
  {
    assert( !map.isNull() );
    assert( fileHeader.seemsValid() );
    assert( index < fileHeader.phnum );
    assert( map.size >= minimumSizeToExtractProgramHeaderAt(fileHeader, index) );

    const ByteArraySpan headerArray = map.subSpan(static_cast<int64_t>(fileHeader.phoff) + index * fileHeader.phentsize, fileHeader.phentsize);

    return programHeaderFromArray(headerArray, fileHeader);
  }

  /*! \internal
   *
   * \pre \a map must not be null
   * \pre \a map must be big enough to read all program headers
   */
  inline
  ProgramHeaderTable extractAllProgramHeaders(const ByteArraySpan & map, const FileHeader & fileHeader)
  {
    assert( !map.isNull() );
    assert( map.size >= fileHeader.minimumSizeToReadAllProgramHeaders() );

    ProgramHeaderTable programHeaders;

    const uint16_t programHeaderCount = fileHeader.phnum;

    for(uint16_t i = 0; i < programHeaderCount; ++i){
      programHeaders.addHeaderFromFile( extractProgramHeaderAt(map, fileHeader, i) );
    }

    return programHeaders;
  }

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_READER_H

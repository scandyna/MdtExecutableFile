// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ELF_SECTION_HEADER_TEST_UTILS_H
#define ELF_SECTION_HEADER_TEST_UTILS_H

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include <string>

using Mdt::ExecutableFile::Elf::SectionHeader;

inline
SectionHeader makeNullSectionHeader()
{
  SectionHeader header;
  header.type = 0;
  header.offset = 0;
  header.addr = 0;
  header.size = 0;

  return header;
}

inline
SectionHeader makeDynamicSectionHeader()
{
  SectionHeader header;
  header.name = ".dynamic";
  header.type = 6;
  header.link = 0;

  return header;
}

inline
SectionHeader makeStringTableSectionHeader(const std::string & name = "")
{
  SectionHeader header;
  header.name = name;
  header.type = 3;

  return header;
}

inline
SectionHeader makeDynamicStringTableSectionHeader()
{
  return makeStringTableSectionHeader(".dynstr");
}

inline
SectionHeader makeDynamicLinkerSymbolTableSectionHeader()
{
  SectionHeader header;
  header.name = ".dynsym";
  header.type = 0xB;
  header.link = 0;

  return header;
}

inline
SectionHeader makeSymbolTableSectionHeader()
{
  SectionHeader header;
  header.name = ".symtab";
  header.type = 0x2;
  header.link = 0;

  return header;
}

inline
SectionHeader makeGlobalOffsetTableSectionHeader(const std::string & name)
{
  SectionHeader header;
  header.type = 1;
  header.name = name;
  header.offset = 0;
  header.size = 0;

  return header;
}

inline
SectionHeader makeGotSectionHeader()
{
  return makeGlobalOffsetTableSectionHeader(".got");
}

inline
SectionHeader makeGotPltSectionHeader()
{
  return makeGlobalOffsetTableSectionHeader(".got.plt");
}

inline
SectionHeader makeProgramInterpreterSectionHeader()
{
  SectionHeader header;
  header.type = 1;
  header.name = ".interp";
  header.offset = 0;
  header.size = 0;

  return header;
}

inline
SectionHeader makeGnuHashTableSectionHeader()
{
  SectionHeader header;
  header.type = 0x6ffffff6;
  header.name = ".gnu.hash";
  header.offset = 0;
  header.size = 0;

  return header;
}

inline
SectionHeader makeNoteSectionHeader(const std::string & name)
{
  SectionHeader header;
  header.type = 7;
  header.name = name;
  header.offset = 0;
  header.size = 0;

  return header;
}

inline
SectionHeader makeRelocationWithAddendSectionHeader(const std::string & name)
{
  SectionHeader header;
  header.type = 4;
  header.name = name;

  return header;
}

#endif // #ifndef ELF_SECTION_HEADER_TEST_UTILS_H

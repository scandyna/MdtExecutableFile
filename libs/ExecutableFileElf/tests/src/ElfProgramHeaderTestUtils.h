// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ELF_PROGRAM_HEADER_TEST_UTILS_H
#define ELF_PROGRAM_HEADER_TEST_UTILS_H

#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeNullProgramHeader()
{
  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.type = 0;

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeDynamicSectionProgramHeader()
{
  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.type = 2;

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeProgramInterpreterProgramHeader()
{
  using Mdt::ExecutableFile::Elf::SegmentType;

  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.setSegmentType(SegmentType::Interpreter);

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeNoteProgramHeader()
{
  using Mdt::ExecutableFile::Elf::SegmentType;

  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.setSegmentType(SegmentType::Note);

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeProgramHeaderTableProgramHeader()
{
  using Mdt::ExecutableFile::Elf::SegmentType;

  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.setSegmentType(SegmentType::ProgramHeaderTable);
  header.memsz = 0;
  header.filesz = 0;

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeGnuRelRoProgramHeader()
{
  using Mdt::ExecutableFile::Elf::SegmentType;

  Mdt::ExecutableFile::Elf::ProgramHeader header;
  header.setSegmentType(SegmentType::GnuRelRo);

  return header;
}

#endif // #ifndef ELF_PROGRAM_HEADER_TEST_UTILS_H

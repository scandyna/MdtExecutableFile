// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Mdt/ExecutableFile/Elf/SymbolTable.h"

using Mdt::ExecutableFile::Elf::SymbolTableEntry;
using Mdt::ExecutableFile::Elf::PartialSymbolTableEntry;
using Mdt::ExecutableFile::Elf::PartialSymbolTableEntry;

SymbolTableEntry makeSectionAssociationSymbolTableEntry()
{
  SymbolTableEntry entry;
  entry.info = 3;

  return entry;
}

PartialSymbolTableEntry makeSectionAssociationSymbolTableEntryWithFileOffset(uint64_t offset)
{
  PartialSymbolTableEntry pEntry;
  pEntry.fileOffset = static_cast<int64_t>(offset);
  pEntry.entry = makeSectionAssociationSymbolTableEntry();

  return pEntry;
}

SymbolTableEntry makeObjectSymbolTableEntry()
{
  SymbolTableEntry entry;
  entry.info = 1;

  return entry;
}

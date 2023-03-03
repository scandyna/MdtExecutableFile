// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ElfFileIoTestCommon.h"
#include "ByteArraySpanTestUtils.h"

// #include "TestUtils.h"

#include "Mdt/ExecutableFile/Elf/DynamicSection.h"

using namespace Mdt::ExecutableFile;
using Mdt::ExecutableFile::Elf::DynamicStruct;
using Mdt::ExecutableFile::Elf::DynamicSection;
using Mdt::ExecutableFile::Elf::StringTable;
using Mdt::ExecutableFile::Elf::DynamicSectionTagType;

DynamicStruct makeEntry(DynamicSectionTagType tag, uint64_t valOrPtr)
{
  DynamicStruct entry(tag);
  entry.val_or_ptr = valOrPtr;

  return entry;
}

DynamicStruct makeNullEntry()
{
  return makeEntry(DynamicSectionTagType::Null, 0);
}

DynamicStruct makeNeededEntry(uint64_t val = 2)
{
  return makeEntry(DynamicSectionTagType::Needed, val);
}

DynamicStruct makeSoNameEntry(uint64_t val = 2)
{
  return makeEntry(DynamicSectionTagType::SoName, val);
}

DynamicStruct makeRunPathEntry(uint64_t val = 2)
{
  return makeEntry(DynamicSectionTagType::Runpath, val);
}

inline
DynamicStruct makeGnuHashEntry(uint64_t address)
{
  return makeEntry(DynamicSectionTagType::GnuHash, address);
}

DynamicStruct makeStringTableAddressEntry(uint64_t address)
{
  return makeEntry(DynamicSectionTagType::StringTable, address);
}

DynamicStruct makeStringTableSizeEntry(uint64_t size)
{
  return makeEntry(DynamicSectionTagType::StringTableSize, size);
}

StringTable stringTableFromCharArray(unsigned char * const array, qint64 size)
{
  return StringTable::fromCharArray( arraySpanFromArray(array, size) );
}

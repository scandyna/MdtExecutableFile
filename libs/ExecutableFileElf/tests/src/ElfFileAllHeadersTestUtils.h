// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ElfProgramHeaderTestUtils.h"
#include "ElfSectionHeaderTestUtils.h"
#include "ElfFileIoTestUtils.h"
#include "Mdt/ExecutableFile/Elf/FileAllHeaders.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/SectionSegmentUtils.h"
#include <cstdint>
#include <cassert>

struct TestHeadersSetup
{
  uint64_t programHeaderTableOffset = 0;
  uint64_t sectionHeaderTableOffset = 0;
  bool sortSectionHeaderTableByFileOffset = false;
  uint64_t programInterpreterSectionOffset = 0;
  uint64_t programInterpreterSectionAddress = 0;
  uint64_t programInterpreterSectionSize = 0;
  uint64_t noteAbiTagSectionOffset = 0;
  uint64_t noteAbiTagSectionAddress = 0;
  uint64_t noteAbiTagSectionSize = 0;
  uint64_t noteGnuBuilIdSectionOffset = 0;
  uint64_t noteGnuBuilIdSectionAddress = 0;
  uint64_t noteGnuBuilIdSectionSize = 0;
  uint64_t gnuHashTableSectionOffset = 0;
  uint64_t gnuHashTableSectionAddress = 0;
  uint64_t gnuHashTableSectionSize = 0;
  uint64_t dynamicSectionOffset = 0;
  uint64_t dynamicSectionSize = 0;
  uint64_t dynamicSectionAddress = 0;
  uint64_t dynamicSectionAlignment = 0;
  uint64_t dynamicStringTableOffset = 0;
  uint64_t dynamicStringTableSize = 0;
  uint64_t dynamicStringTableAddress = 0;
  uint64_t gotPltSectionOffset = 0;
  uint64_t gotPltSectionAddress = 0;
  uint64_t gotPltSectionSize = 0;
  uint64_t sectionNameStringTableOffset = 0;

  bool containsProgramHeaderTable() const noexcept
  {
    if(programHeaderTableOffset == 0){
      return false;
    }
    return true;
  }

  bool containsSectionHeaderTable() const noexcept
  {
    if(sectionHeaderTableOffset == 0){
      return false;
    }
    return true;
  }

  bool containsProgramInterpreter() const noexcept
  {
    if(programInterpreterSectionOffset == 0){
      return false;
    }
    if(programInterpreterSectionAddress == 0){
      return false;
    }
    if(programInterpreterSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsNoteAbiTag() const noexcept
  {
    if(noteAbiTagSectionOffset == 0){
      return false;
    }
    if(noteAbiTagSectionAddress == 0){
      return false;
    }
    if(noteAbiTagSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsNoteGnuBuildId() const noexcept
  {
    if(noteGnuBuilIdSectionOffset == 0){
      return false;
    }
    if(noteGnuBuilIdSectionAddress == 0){
      return false;
    }
    if(noteGnuBuilIdSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsGnuHashTable() const noexcept
  {
    if(gnuHashTableSectionOffset == 0){
      return false;
    }
    if(gnuHashTableSectionAddress == 0){
      return false;
    }
    if(gnuHashTableSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsDynamicSection() const noexcept
  {
    if(dynamicSectionOffset == 0){
      return false;
    }
    if(dynamicSectionAddress == 0){
      return false;
    }
    if(dynamicSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsDynamicStringTable() const noexcept
  {
    if(dynamicStringTableOffset == 0){
      return false;
    }
    if(dynamicStringTableAddress == 0){
      return false;
    }
    if(dynamicStringTableSize == 0){
      return false;
    }
    return true;
  }

  bool containsGotPlt() const noexcept
  {
    if(gotPltSectionOffset == 0){
      return false;
    }
    if(gotPltSectionAddress == 0){
      return false;
    }
    if(gotPltSectionSize == 0){
      return false;
    }
    return true;
  }

  bool containsSectionNameStringTable() const noexcept
  {
    if(sectionNameStringTableOffset == 0){
      return false;
    }
    return true;
  }
};


inline
Mdt::ExecutableFile::Elf::ProgramHeader makeProgramInterpreterProgramHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::ProgramHeader;

  assert( setup.containsProgramInterpreter() );

  ProgramHeader header = makeProgramInterpreterProgramHeader();
  header.offset = setup.programInterpreterSectionOffset;
  header.filesz = setup.programInterpreterSectionSize;
  header.vaddr = setup.programInterpreterSectionAddress;
  header.paddr = setup.programInterpreterSectionAddress;
  header.align = 1;
  header.paddr = setup.programInterpreterSectionAddress;
  header.memsz = setup.programInterpreterSectionSize;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeProgramInterpreterSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsProgramInterpreter() );

  SectionHeader header = makeProgramInterpreterSectionHeader();
  header.name = ".interp";
  header.offset = setup.programInterpreterSectionOffset;
  header.size = setup.programInterpreterSectionSize;
  header.addr = setup.programInterpreterSectionAddress;
  header.addralign = 1;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeNoteAbiTagSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsNoteAbiTag() );

  SectionHeader header = makeNoteSectionHeader(".note.ABI-tag");
  header.offset = setup.noteAbiTagSectionOffset;
  header.size = setup.noteAbiTagSectionSize;
  header.addr = setup.noteAbiTagSectionAddress;
  header.addralign = 4;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeNoteGnuBuildIdSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsNoteGnuBuildId() );

  SectionHeader header = makeNoteSectionHeader(".note.gnu.build-id");
  header.offset = setup.noteGnuBuilIdSectionOffset;
  header.size = setup.noteGnuBuilIdSectionSize;
  header.addr = setup.noteGnuBuilIdSectionAddress;
  header.addralign = 4;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeGnuHashTableSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsGnuHashTable() );

  SectionHeader header = makeGnuHashTableSectionHeader();
  header.offset = setup.gnuHashTableSectionOffset;
  header.addr = setup.gnuHashTableSectionAddress;
  header.size = setup.gnuHashTableSectionSize;
  header.addralign = 8;

  return header;
}

inline
Mdt::ExecutableFile::Elf::ProgramHeader makeDynamicSectionProgramHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::ProgramHeader;

  assert( setup.containsDynamicSection() );
//   assert( setup.dynamicSectionAlignment > 0 );

  ProgramHeader header = makeDynamicSectionProgramHeader();
  header.offset = setup.dynamicSectionOffset;
  header.filesz = setup.dynamicSectionSize;
  header.vaddr = setup.dynamicSectionAddress;
  header.paddr = setup.dynamicSectionAddress;
  header.align = setup.dynamicSectionAlignment;
  header.paddr = setup.dynamicSectionAddress;
  header.memsz = setup.dynamicSectionSize;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeDynamicSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsDynamicSection() );
//   assert( setup.dynamicSectionAlignment > 0 );

  SectionHeader header = makeDynamicSectionHeader();
  header.name = ".dynamic";
  header.offset = setup.dynamicSectionOffset;
  header.size = setup.dynamicSectionSize;
  header.addr = setup.dynamicSectionAddress;
  header.addralign = setup.dynamicSectionAlignment;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeDynamicStringTableSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsDynamicStringTable() );

  SectionHeader header = makeStringTableSectionHeader();
  header.name = ".dynstr";
  header.offset = setup.dynamicStringTableOffset;
  header.size = setup.dynamicStringTableSize;
  header.addr = setup.dynamicStringTableAddress;
  header.addralign = 1;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeGotPltSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsGotPlt() );

  SectionHeader header = makeGotPltSectionHeader();
  header.offset = setup.gotPltSectionOffset;
  header.size = setup.gotPltSectionSize;
  header.addr = setup.gotPltSectionAddress;

  return header;
}

inline
Mdt::ExecutableFile::Elf::SectionHeader makeSectionNameStringTableSectionHeader(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::SectionHeader;

  assert( setup.containsSectionNameStringTable() );

  SectionHeader header = makeStringTableSectionHeader();
  header.name = ".shstrtab";
  header.offset = setup.sectionNameStringTableOffset;
  header.size = 100;
  header.addr = 0;

  return header;
}


inline
Mdt::ExecutableFile::Elf::FileAllHeaders makeTestHeaders(const TestHeadersSetup & setup)
{
  using Mdt::ExecutableFile::Elf::MoveSectionAlignment;
  using Mdt::ExecutableFile::Elf::FileHeader;
  using Mdt::ExecutableFile::Elf::ProgramHeader;
  using Mdt::ExecutableFile::Elf::SegmentType;
  using Mdt::ExecutableFile::Elf::ProgramHeaderTable;
  using Mdt::ExecutableFile::Elf::SectionHeader;
  using Mdt::ExecutableFile::Elf::FileAllHeaders;
  using Mdt::ExecutableFile::Elf::sortSectionHeadersByFileOffset;
  using Mdt::ExecutableFile::Elf::makeNoteProgramHeaderCoveringSections;

  FileAllHeaders headers;
  FileHeader fileHeader = make64BitLittleEndianFileHeader();
  fileHeader.phoff = setup.programHeaderTableOffset;
  fileHeader.shoff = setup.sectionHeaderTableOffset;

  SectionHeader noteAbiTagSectionHeader;
  if( setup.containsNoteAbiTag() ){
    noteAbiTagSectionHeader = makeNoteAbiTagSectionHeader(setup);
  }

  SectionHeader noteGnuBuildIdSectionHeader;
  if( setup.containsNoteGnuBuildId() ){
    noteGnuBuildIdSectionHeader = makeNoteGnuBuildIdSectionHeader(setup);
  }

  ProgramHeader noteProgramHeader;
  if( setup.containsNoteAbiTag() && !setup.containsNoteGnuBuildId() ){
    noteProgramHeader = makeNoteProgramHeaderCoveringSections({noteAbiTagSectionHeader});
  }else if( !setup.containsNoteAbiTag() && setup.containsNoteGnuBuildId() ){
    noteProgramHeader = makeNoteProgramHeaderCoveringSections({noteGnuBuildIdSectionHeader});
  }else if( setup.containsNoteAbiTag() && setup.containsNoteGnuBuildId() ){
    noteProgramHeader = makeNoteProgramHeaderCoveringSections({noteAbiTagSectionHeader, noteGnuBuildIdSectionHeader});
  }

  ProgramHeader programHeaderTableProgramHeader = makeProgramHeaderTableProgramHeader();
  programHeaderTableProgramHeader.offset = setup.programHeaderTableOffset;
  programHeaderTableProgramHeader.vaddr = setup.programHeaderTableOffset;
  programHeaderTableProgramHeader.paddr = setup.programHeaderTableOffset;

  const uint16_t programHeaderSize = 56;
  ProgramHeaderTable programHeaderTable;
  programHeaderTable.addHeader(programHeaderTableProgramHeader, programHeaderSize);
  if( setup.containsProgramInterpreter() ){
    programHeaderTable.addHeader(makeProgramInterpreterProgramHeader(setup), programHeaderSize);
  }
  if( setup.containsNoteAbiTag() || setup.containsNoteGnuBuildId() ){
    programHeaderTable.addHeader(noteProgramHeader, programHeaderSize);
  }
  if( setup.containsDynamicSection() ){
    programHeaderTable.addHeader(makeDynamicSectionProgramHeader(setup), programHeaderSize);
  }

  std::vector<SectionHeader> sectionHeaderTable;
  sectionHeaderTable.push_back( makeNullSectionHeader() );
  if( setup.containsProgramInterpreter() ){
    sectionHeaderTable.push_back( makeProgramInterpreterSectionHeader(setup) );
  }
  if( setup.containsNoteAbiTag() ){
    sectionHeaderTable.push_back(noteAbiTagSectionHeader);
  }
  if( setup.containsNoteGnuBuildId() ){
    sectionHeaderTable.push_back(noteGnuBuildIdSectionHeader);
  }
  if( setup.containsGnuHashTable() ){
    sectionHeaderTable.push_back( makeGnuHashTableSectionHeader(setup) );
  }
  if( setup.containsDynamicSection() ){
    SectionHeader dynamicSectionHeader = makeDynamicSectionHeader(setup);
    if( setup.containsDynamicStringTable() ){
      dynamicSectionHeader.link = static_cast<uint32_t>(sectionHeaderTable.size()+1);
    }else{
      dynamicSectionHeader.link = 0;
    }
    sectionHeaderTable.push_back(dynamicSectionHeader);
  }
  if( setup.containsDynamicStringTable() ){
    sectionHeaderTable.push_back( makeDynamicStringTableSectionHeader(setup) );
  }
  if( setup.containsGotPlt() ){
    sectionHeaderTable.push_back( makeGotPltSectionHeader(setup) );
  }
  if( setup.containsSectionNameStringTable() ){
    sectionHeaderTable.push_back( makeSectionNameStringTableSectionHeader(setup) );
    fileHeader.shstrndx = static_cast<uint16_t>( sectionHeaderTable.size() - 1 );
  }else{
    fileHeader.shstrndx = 0;
  }

  if(setup.sortSectionHeaderTableByFileOffset){
    sortSectionHeadersByFileOffset(sectionHeaderTable);
  }

//   assert( !sectionHeaderTable.empty() );
//   fileHeader.shstrndx = static_cast<uint16_t>( sectionHeaderTable.size() - 1 );

  headers.setFileHeader(fileHeader);
  headers.setProgramHeaderTable(programHeaderTable);
  headers.setSectionHeaderTable(sectionHeaderTable);

  return headers;
}

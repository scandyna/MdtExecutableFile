// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ELF_FILE_IO_TEST_UTILS_H
#define ELF_FILE_IO_TEST_UTILS_H

// #include "Mdt/DeployUtils/Impl/Elf/FileReader.h"
#include "Mdt/ExecutableFile/Elf/Ident.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <QString>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace Mdt::ExecutableFile;

Elf::Ident makeValidIdent(Elf::Class _class, Elf::DataFormat dataFormat)
{
  Elf::Ident ident;

  ident.hasValidElfMagicNumber = true;
  ident._class = _class;
  ident.dataFormat = dataFormat;
  ident.version = 1;
  ident.osabi = 0;
  ident.abiversion = 0;

  assert( ident.isValid() );

  return ident;
}

Elf::Ident make32BitLittleEndianIdent()
{
  using Elf::Class;
  using Elf::DataFormat;

  return makeValidIdent(Class::Class32, DataFormat::Data2LSB);
}

Elf::Ident make32BitBigEndianIdent()
{
  using Elf::Class;
  using Elf::DataFormat;

  return makeValidIdent(Class::Class32, DataFormat::Data2MSB);
}

Elf::Ident make64BitLittleEndianIdent()
{
  using Elf::Class;
  using Elf::DataFormat;

  return makeValidIdent(Class::Class64, DataFormat::Data2LSB);
}

Elf::Ident make64BitBigEndianIdent()
{
  using Elf::Class;
  using Elf::DataFormat;

  return makeValidIdent(Class::Class64, DataFormat::Data2MSB);
}

Elf::FileHeader make32BitBigEndianFileHeader()
{
  using Elf::ObjectFileType;
  using Elf::Machine;

  Elf::FileHeader fileHeader;

  fileHeader.ident = make32BitBigEndianIdent();
  fileHeader.setObjectFileType(ObjectFileType::SharedObject);
  fileHeader.setMachineType(Machine::X86);
  fileHeader.version = 1;
  fileHeader.entry = 100;
  fileHeader.phoff = 0x34;
  fileHeader.shoff = 1000;
  fileHeader.flags = 0;
  fileHeader.ehsize = 52;
  fileHeader.phentsize = 32;
  fileHeader.phnum = 9;
  fileHeader.shentsize = 40;
  fileHeader.shnum = 10;
  fileHeader.shstrndx = 9;

  return fileHeader;
}

Elf::FileHeader make64BitLittleEndianFileHeader()
{
  using Elf::ObjectFileType;
  using Elf::Machine;

  Elf::FileHeader fileHeader;

  fileHeader.ident = make64BitLittleEndianIdent();
  fileHeader.setObjectFileType(ObjectFileType::SharedObject);
  fileHeader.setMachineType(Machine::X86_64);
  fileHeader.version = 1;
  fileHeader.entry = 100;
  fileHeader.phoff = 0x40;
  fileHeader.shoff = 1000;
  fileHeader.flags = 0;
  fileHeader.ehsize = 64;
  fileHeader.phentsize = 56;
  fileHeader.phnum = 9;
  fileHeader.shentsize = 64;
  fileHeader.shnum = 10;
  fileHeader.shstrndx = 9;

  return fileHeader;
}

#endif // #ifndef ELF_FILE_IO_TEST_UTILS_H

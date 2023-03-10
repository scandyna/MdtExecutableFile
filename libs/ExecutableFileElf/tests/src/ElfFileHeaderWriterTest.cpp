// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "ElfFileIoTestUtils.h"
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/Elf/FileHeaderWriter.h"

using Mdt::ExecutableFile::ByteArraySpan;
using Mdt::ExecutableFile::Elf::FileHeader;
using Mdt::ExecutableFile::Elf::Ident;
using Mdt::ExecutableFile::Elf::Class;
using Mdt::ExecutableFile::Elf::DataFormat;
using Mdt::ExecutableFile::Elf::ObjectFileType;
using Mdt::ExecutableFile::Elf::Machine;

TEST_CASE("identToArray")
{
  using Mdt::ExecutableFile::Elf::identToArray;

  Ident ident;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  ident.hasValidElfMagicNumber = true;
  ident._class = Class::Class32;
  ident.dataFormat = DataFormat::Data2MSB;
  ident.version = 1;
  ident.osabi = 0x03;
  ident.abiversion = 1;

  uchar expectedArrayData[16] = {
    // magic number
    0x7F,'E','L','F',
    // Class (32 or 64 bit)
    1, // 32
    // Data format
    2,  // big-endian
    // Version
    1,
    // OS ABI
    0x03, // Linux
    // ABI version
    1,
    // Padding
    0,0,0,0,0,0,0
  };
  expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

  uchar arrayData[16] = {0,0,0,0,0,0,0,0,0,7,6,5,4,3,2,1};
  array = arraySpanFromArray( arrayData, sizeof(arrayData) );

  identToArray(array, ident);
  REQUIRE( arraysAreEqual(array, expectedArray) );
}

TEST_CASE("fileHeaderToArray")
{
  using Mdt::ExecutableFile::Elf::fileHeaderToArray;

  FileHeader header;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  SECTION("32-bit big-endian")
  {
    header.ident.hasValidElfMagicNumber = true;
    header.ident._class = Class::Class32;
    header.ident.dataFormat = DataFormat::Data2MSB;
    header.ident.version = 1;
    header.ident.osabi = 0;
    header.ident.abiversion = 0;
    header.setObjectFileType(ObjectFileType::SharedObject);
    header.setMachineType(Machine::X86);
    header.version = 1;
    header.entry = 0x3210;
    header.phoff = 0x34;
    header.shoff = 0x12345678;
    header.flags = 0x12;
    header.ehsize = 52;
    header.phentsize = 56;
    header.phnum = 7;
    header.shentsize = 32;
    header.shnum = 35;
    header.shstrndx = 34;

    uchar expectedArrayData[52] = {
      // e_ident
        // magic number
        0x7F,'E','L','F',
        // Class (32 or 64 bit)
        1, // 32
        // Data format
        2,  // big-endian
        // Version
        1,
        // OS ABI
        0x00, // System V
        // ABI version
        0,
        // Padding
        0,0,0,0,0,0,0,
      // e_type
      0,0x03, // ET_DYN (shared object)
      // e_machine
      0,0x03, // x86
      // e_version
      0,0,0,1, // 1
      // e_entry
      0,0,0x32,0x10, // 0x3210
      // e_phoff
      0,0,0,0x34, // 0x34
      // e_shoff
      0x12,0x34,0x56,0x78, // 0x12345678
      // e_flags
      0,0,0,0x12, // Some flags
      // e_ehsize
      0,52, // 52 bytes
      // e_phentsize
      0,56, // 56 bytes
      // e_phnum
      0,7, // 7 entries in program header table
      // e_shentsize
      0,32, // 32 bytes
      // e_shnum
      0,35, // 35 entries in section header table
      // e_shstrndx
      0,34
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    uchar arrayData[52] = {};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    fileHeaderToArray(array, header);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }

  SECTION("64-bit little-endian")
  {
    header.ident.hasValidElfMagicNumber = true;
    header.ident._class = Class::Class64;
    header.ident.dataFormat = DataFormat::Data2LSB;
    header.ident.version = 1;
    header.ident.osabi = 0;
    header.ident.abiversion = 0;
    header.setObjectFileType(ObjectFileType::SharedObject);
    header.setMachineType(Machine::X86_64);
    header.version = 1;
    header.entry = 0x3210;
    header.phoff = 0x40;
    header.shoff = 0x1234567890;
    header.flags = 0x12;
    header.ehsize = 64;
    header.phentsize = 56;
    header.phnum = 7;
    header.shentsize = 32;
    header.shnum = 35;
    header.shstrndx = 34;

    uchar expectedArrayData[64] = {
      // e_ident
        // magic number
        0x7F,'E','L','F',
        // Class (32 or 64 bit)
        2, // 64 bit
        // Data format
        1,  // little-endian
        // Version
        1,
        // OS ABI
        0x00, // System V
        // ABI version
        0,
        // Padding
        0,0,0,0,0,0,0,
      // e_type
      0x03,0, // ET_DYN (shared object)
      // e_machine
      0x3E,0, // x86-64
      // e_version
      1,0,0,0, // 1
      // e_entry
      0x10,0x32,0,0,0,0,0,0, // 0x3210
      // e_phoff
      0x40,0,0,0,0,0,0,0, // 0x40
      // e_shoff
      0x90,0x78,0x56,0x34,0x12,0,0,0, // 0x1234567890
      // e_flags
      0x12,0,0,0, // Some flags
      // e_ehsize
      64,0, // 64 bytes
      // e_phentsize
      56,0, // 56 bytes
      // e_phnum
      7,0, // 7 entries in program header table
      // e_shentsize
      32,0, // 32 bytes
      // e_shnum
      35,0, // 35 entries in section header table
      // e_shstrndx
      34,0
    };
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    uchar arrayData[64] = {};
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    fileHeaderToArray(array, header);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }
}

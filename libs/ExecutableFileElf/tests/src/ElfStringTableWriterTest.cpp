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
#include "ElfFileIoTestCommon.h"
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/Elf/StringTable.h"
#include "Mdt/ExecutableFile/Elf/StringTableWriter.h"

using Mdt::ExecutableFile::ByteArraySpan;
using Mdt::ExecutableFile::Elf::FileHeader;
using Mdt::ExecutableFile::Elf::Ident;
using Mdt::ExecutableFile::Elf::StringTable;

TEST_CASE("stringTableToArray")
{
  using Mdt::ExecutableFile::Elf::stringTableToArray;

  StringTable stringTable;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  uchar expectedArrayData[14] = {'\0','/','t','m','p','\0','l','i','b','A','.','s','o','\0'};
  expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

  stringTable = StringTable::fromCharArray(expectedArray);

  uchar arrayData[14] = {};
  array = arraySpanFromArray( arrayData, sizeof(arrayData) );

  stringTableToArray(array, stringTable);
  REQUIRE( arraysAreEqual(array, expectedArray) );
}

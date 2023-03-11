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
#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSectionReader.h"
#include "Mdt/ExecutableFile/Elf/ProgramInterpreterSectionWriter.h"

using namespace Mdt::ExecutableFile::Elf;
using Mdt::ExecutableFile::ByteArraySpan;


TEST_CASE("setProgramInterpreterSectionToArray")
{
  ProgramInterpreterSection section;
  ByteArraySpan array;
  ByteArraySpan expectedArray;

  SECTION("A")
  {
    section.path = "A";

    uchar expectedArrayData[2] = {'A','\0'};
    expectedArray = arraySpanFromArray( expectedArrayData, sizeof(expectedArrayData) );

    uchar arrayData[2];
    array = arraySpanFromArray( arrayData, sizeof(arrayData) );

    setProgramInterpreterSectionToArray(array, section);
    REQUIRE( arraysAreEqual(array, expectedArray) );
  }
}

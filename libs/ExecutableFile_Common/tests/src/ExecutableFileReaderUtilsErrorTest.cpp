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
#include "Mdt/ExecutableFile/ExecutableFileReaderUtils.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("qStringFromUft8UnsignedCharArray")
{
  ByteArraySpan span;

  SECTION("A (no end of string)")
  {
    unsigned char array[1] = {'A'};
    span.data = array;
    span.size = 1;
    REQUIRE_THROWS_AS( qStringFromUft8UnsignedCharArray(span), NotNullTerminatedStringError );
  }
}

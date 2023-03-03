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
#include "ByteArraySpanTestUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileReaderUtils.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("stringFromBoundedUnsignedCharArray")
{
  ByteArraySpan span;

  SECTION("span is larger that string")
  {
    SECTION("A")
    {
      unsigned char array[2] = {'A','\0'};
      span = arraySpanFromArray( array, sizeof(array) );

      REQUIRE( stringFromBoundedUnsignedCharArray(span) == "A" );
    }
  }

  SECTION("span is just large enought")
  {
    SECTION("A (string not null terminated)")
    {
      unsigned char array[1] = {'A'};
      span = arraySpanFromArray( array, sizeof(array) );

      REQUIRE( stringFromBoundedUnsignedCharArray(span) == "A" );
    }

    SECTION("A (string is null terminated)")
    {
      unsigned char array[2] = {'A','\0'};
      span = arraySpanFromArray( array, sizeof(array) );

      REQUIRE( stringFromBoundedUnsignedCharArray(span) == "A" );
    }
  }
}

TEST_CASE("containsEndOfString")
{
  ByteArraySpan span;

  SECTION("empty")
  {
    unsigned char array[1] = {};
    span.data = array;

    SECTION("size: 0")
    {
      span.size = 0;
      REQUIRE( !containsEndOfString(span) );
    }
  }

  SECTION("ABC")
  {
    unsigned char array[4] = {'A','B','C','\0'};
    span.data = array;

    SECTION("size: 3")
    {
      span.size = 3;
      REQUIRE( !containsEndOfString(span) );
    }

    SECTION("size: 4")
    {
      span.size = 4;
      REQUIRE( containsEndOfString(span) );
    }
  }
}

TEST_CASE("qStringFromUft8UnsignedCharArray")
{
  ByteArraySpan span;

  SECTION("A")
  {
    unsigned char array[2] = {'A','\0'};
    span.data = array;
    span.size = 2;
    REQUIRE( qStringFromUft8UnsignedCharArray(span) == QLatin1String("A") );
  }
}

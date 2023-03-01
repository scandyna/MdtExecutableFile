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
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <algorithm>
#include <vector>

using namespace Mdt::ExecutableFile;


ByteArraySpan spanFromArray(unsigned char * const data, int64_t size)
{
  ByteArraySpan span;

  span.data = data;
  span.size = size;

  return span;
}

TEST_CASE("Construct")
{
  SECTION("default constructed")
  {
    ByteArraySpan span;
    REQUIRE( span.isNull() );
  }
}

TEST_CASE("isInRange")
{
  unsigned char array[4] = {1,2,3,4};
  const ByteArraySpan span = spanFromArray( array, sizeof(array) );

  SECTION("offset 0, size 1")
  {
    REQUIRE( span.isInRange(0, 1) );
  }

  SECTION("offset 1, size 3")
  {
    REQUIRE( span.isInRange(1, 3) );
  }

  SECTION("offset 1, size 4")
  {
    REQUIRE( !span.isInRange(1, 4) );
  }
}

TEST_CASE("subSpan")
{
  unsigned char array[4] = {1,2,3,4};
  const ByteArraySpan span = spanFromArray( array, sizeof(array) );

  SECTION("offset 0, size 1")
  {
    const ByteArraySpan subSpan = span.subSpan(0, 1);
    REQUIRE( subSpan.size == 1 );
    REQUIRE( subSpan.data[0] == 1 );
  }

  SECTION("offset 1, size 3")
  {
    const ByteArraySpan subSpan = span.subSpan(1, 3);
    REQUIRE( subSpan.size == 3 );
    REQUIRE( subSpan.data[0] == 2 );
    REQUIRE( subSpan.data[1] == 3 );
    REQUIRE( subSpan.data[2] == 4 );
  }

  SECTION("get remaining from offset 1")
  {
    const ByteArraySpan subSpan = span.subSpan(1);
    REQUIRE( subSpan.size == 3 );
    REQUIRE( subSpan.data[0] == 2 );
    REQUIRE( subSpan.data[1] == 3 );
    REQUIRE( subSpan.data[2] == 4 );
  }
}

TEST_CASE("std_copy")
{
  unsigned char array[4] = {1,2,3,4};
  const ByteArraySpan span = spanFromArray( array, sizeof(array) );
  std::vector<uchar> v(4);

  std::copy( span.cbegin(), span.cend(), v.begin() );
  REQUIRE( v[0] == 1 );
  REQUIRE( v[1] == 2 );
  REQUIRE( v[2] == 3 );
  REQUIRE( v[3] == 4 );
}

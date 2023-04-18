// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Numeric.h"

using Catch::Matchers::WithinRel;


TEST_CASE("qulonglong_from_uint64")
{
  SECTION("0")
  {
    REQUIRE( qulonglong_from_uint64(0) == 0 );
  }

  SECTION("1")
  {
    REQUIRE( qulonglong_from_uint64(1) == 1 );
  }

  SECTION("max")
  {
    REQUIRE( qulonglong_from_uint64(0xffffffffffffffff) == 0xffffffffffffffff );
  }
}

TEST_CASE("qulonglong_from_size_t")
{
  SECTION("0")
  {
    REQUIRE( qulonglong_from_size_t(0) == 0 );
  }

  SECTION("1")
  {
    REQUIRE( qulonglong_from_size_t(1) == 1 );
  }

  SECTION("max")
  {
    REQUIRE( qulonglong_from_size_t(0xffffffffffffffff) == 0xffffffffffffffff );
  }
}

TEST_CASE("size_t_from_qulonglong")
{
  SECTION("0")
  {
    REQUIRE( size_t_from_qulonglong(0) == 0 );
  }

  SECTION("1")
  {
    REQUIRE( size_t_from_qulonglong(1) == 1 );
  }

  SECTION("max")
  {
    REQUIRE( size_t_from_qulonglong(0xffffffffffffffff) == 0xffffffffffffffff );
  }
}

TEST_CASE("qreal_from_uint64")
{
  SECTION("0")
  {
    REQUIRE_THAT( qreal_from_uint64(0), WithinRel(0.0) );
  }

  SECTION("1")
  {
    REQUIRE_THAT( qreal_from_uint64(1), WithinRel(1.0) );
  }

  SECTION("1'000'000'000")
  {
    REQUIRE_THAT( qreal_from_uint64(1'000'000'000), WithinRel(1'000'000'000.0) );
  }
}

TEST_CASE("qreal_from_qulonglong")
{
  SECTION("0")
  {
    REQUIRE_THAT( qreal_from_qulonglong(0), WithinRel(0.0) );
  }

  SECTION("1")
  {
    REQUIRE_THAT( qreal_from_qulonglong(1), WithinRel(1.0) );
  }

  SECTION("1'000'000'000")
  {
    REQUIRE_THAT( qreal_from_qulonglong(1'000'000'000), WithinRel(1'000'000'000.0) );
  }
}

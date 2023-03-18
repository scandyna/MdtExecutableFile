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
#include "RPathTestUtils.h"
#include "Mdt/ExecutableFile/RPath.h"
#include <QLatin1String>

using Mdt::ExecutableFile::RPathEntry;
using Mdt::ExecutableFile::RPath;

TEST_CASE("entry_isRelative")
{
  SECTION("opt")
  {
    RPathEntry entry( QLatin1String("opt") );

    REQUIRE( entry.isRelative() );
  }

  SECTION("./opt")
  {
    RPathEntry entry( QLatin1String("./opt") );

    REQUIRE( entry.isRelative() );
  }

  SECTION("../opt")
  {
    RPathEntry entry( QLatin1String("../opt") );

    REQUIRE( entry.isRelative() );
  }

  SECTION("/opt")
  {
    RPathEntry entry( QLatin1String("/opt") );

    REQUIRE( !entry.isRelative() );
  }
}

TEST_CASE("append_and_attributes")
{
  RPath rpath;

  SECTION("default constructed")
  {
    REQUIRE( rpath.entriesCount() == 0 );
    REQUIRE( rpath.isEmpty() );
  }

  SECTION("1 path")
  {
    rpath.appendPath( QLatin1String(".") );

    REQUIRE( rpath.entriesCount() == 1 );
    REQUIRE( !rpath.isEmpty() );
    REQUIRE( rpath.entryAt(0).path() == QLatin1String(".") );
  }
}

TEST_CASE("clear")
{
  RPath rpath;
  rpath.appendPath( QLatin1String(".") );
  REQUIRE( !rpath.isEmpty() );

  rpath.clear();

  REQUIRE( rpath.isEmpty() );
}

TEST_CASE("RPathEntry_compare_equal")
{
  SECTION("/tmp == /tmp")
  {
    RPathEntry a( QLatin1String("/tmp") );
    RPathEntry b( QLatin1String("/tmp") );

    REQUIRE( a == b );
  }

  SECTION("/tmp == /tmp/")
  {
    RPathEntry a( QLatin1String("/tmp") );
    RPathEntry b( QLatin1String("/tmp/") );

    REQUIRE( a == b );
  }

  SECTION("/tmp/ == /tmp")
  {
    RPathEntry a( QLatin1String("/tmp/") );
    RPathEntry b( QLatin1String("/tmp") );

    REQUIRE( a == b );
  }

  SECTION("/tmp != /lib")
  {
    RPathEntry a( QLatin1String("/tmp") );
    RPathEntry b( QLatin1String("/lib") );

    REQUIRE( !(a == b) );
  }
}

TEST_CASE("RPath_compare_equal")
{
  RPath a, b;

  SECTION("empty")
  {
    REQUIRE( a == b );
  }

  SECTION("/tmp == /tmp")
  {
    a.appendPath( QLatin1String("/tmp") );
    b.appendPath( QLatin1String("/tmp") );

    REQUIRE( a == b );
  }

  SECTION("/tmp != /lib")
  {
    a.appendPath( QLatin1String("/tmp") );
    b.appendPath( QLatin1String("/lib") );

    REQUIRE( !(a == b) );
  }

  SECTION("empty != /lib")
  {
    b.appendPath( QLatin1String("/lib") );

    REQUIRE( !(a == b) );
  }
}

TEST_CASE("RPath_compare_notEqual")
{
  RPath a, b;

  SECTION("/tmp == /tmp")
  {
    a.appendPath( QLatin1String("/tmp") );
    b.appendPath( QLatin1String("/tmp") );

    REQUIRE( !(a != b) );
  }

  SECTION("/tmp != /lib")
  {
    a.appendPath( QLatin1String("/tmp") );
    b.appendPath( QLatin1String("/lib") );

    REQUIRE( a != b );
  }
}

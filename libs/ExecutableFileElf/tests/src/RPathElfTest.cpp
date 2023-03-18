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
#include "Mdt/ExecutableFile/RPathElf.h"
#include <QLatin1String>

using Mdt::ExecutableFile::RPathEntry;
using Mdt::ExecutableFile::RPath;
using Mdt::ExecutableFile::RPathElf;


TEST_CASE("rPathEntryFromString")
{
  SECTION("/lib")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("/lib") );
    REQUIRE( entry.path() == QLatin1String("/lib") );
  }

  SECTION("lib (relative path)")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("lib") );
    REQUIRE( entry.path() == QLatin1String("lib") );
  }

  SECTION("$ORIGIN")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("$ORIGIN") );
    REQUIRE( entry.path() == QLatin1String(".") );
  }

  SECTION("${ORIGIN}")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("${ORIGIN}") );
    REQUIRE( entry.path() == QLatin1String(".") );
  }

  SECTION("ORIGIN")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("ORIGIN") );
    REQUIRE( entry.path() == QLatin1String("ORIGIN") );
  }

  SECTION("$ORIGIN/lib")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("$ORIGIN/lib") );
    REQUIRE( entry.path() == QLatin1String("lib") );
  }

  SECTION("${ORIGIN}/lib")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("${ORIGIN}/lib") );
    REQUIRE( entry.path() == QLatin1String("lib") );
  }

  SECTION("$ORIGIN/../lib")
  {
    const RPathEntry entry = RPathElf::rPathEntryFromString( QLatin1String("$ORIGIN/../lib") );
    REQUIRE( entry.path() == QLatin1String("../lib") );
  }
}

TEST_CASE("rPathFromString")
{
  RPath rpath;

  SECTION("empty")
  {
    rpath = RPathElf::rPathFromString( QString() );
    REQUIRE( rpath.isEmpty() );
  }

  SECTION("/lib")
  {
    rpath = RPathElf::rPathFromString( QLatin1String("/lib") );
    REQUIRE( rpath.entryAt(0).path() == QLatin1String("/lib") );
  }

  SECTION("lib (relative path)")
  {
    rpath = RPathElf::rPathFromString( QLatin1String("lib") );
    REQUIRE( rpath.entryAt(0).path() == QLatin1String("lib") );
  }

  SECTION("$ORIGIN")
  {
    rpath = RPathElf::rPathFromString( QLatin1String("$ORIGIN") );
    REQUIRE( rpath.entryAt(0).path() == QLatin1String(".") );
  }

  SECTION("$ORIGIN:opt/lib")
  {
    rpath = RPathElf::rPathFromString( QLatin1String("$ORIGIN:opt/lib") );
    REQUIRE( rpath.entryAt(0).path() == QLatin1String(".") );
    REQUIRE( rpath.entryAt(1).path() == QLatin1String("opt/lib") );
  }
}

TEST_CASE("rPathEntryToString")
{
  SECTION("/lib")
  {
    RPathEntry rpathEntry( QLatin1String("/lib") );

    REQUIRE( RPathElf::rPathEntryToString(rpathEntry) == QLatin1String("/lib") );
  }

  SECTION(".")
  {
    RPathEntry rpathEntry( QLatin1String(".") );

    REQUIRE( RPathElf::rPathEntryToString(rpathEntry) == QLatin1String("$ORIGIN") );
  }

  SECTION("lib")
  {
    RPathEntry rpathEntry( QLatin1String("lib") );

    REQUIRE( RPathElf::rPathEntryToString(rpathEntry) == QLatin1String("$ORIGIN/lib") );
  }

  SECTION("./lib")
  {
    RPathEntry rpathEntry( QLatin1String("./lib") );

    REQUIRE( RPathElf::rPathEntryToString(rpathEntry) == QLatin1String("$ORIGIN/lib") );
  }

  SECTION("../lib")
  {
    RPathEntry rpathEntry( QLatin1String("../lib") );

    REQUIRE( RPathElf::rPathEntryToString(rpathEntry) == QLatin1String("$ORIGIN/../lib") );
  }
}

TEST_CASE("rPathToString")
{
  RPath rpath;

  SECTION("empty")
  {
    REQUIRE( RPathElf::rPathToString(rpath).isEmpty() );
  }

  SECTION("/lib")
  {
    rpath.appendPath( QLatin1String("/lib") );

    REQUIRE( RPathElf::rPathToString(rpath) == QLatin1String("/lib") );
  }

  SECTION("/lib:opt/lib")
  {
    rpath.appendPath( QLatin1String("/lib") );
    rpath.appendPath( QLatin1String("opt/lib") );

    REQUIRE( RPathElf::rPathToString(rpath) == QLatin1String("/lib:$ORIGIN/opt/lib") );
  }

  SECTION("/lib:opt/lib:lib")
  {
    rpath.appendPath( QLatin1String("/lib") );
    rpath.appendPath( QLatin1String("opt/lib") );
    rpath.appendPath( QLatin1String("lib") );

    REQUIRE( RPathElf::rPathToString(rpath) == QLatin1String("/lib:$ORIGIN/opt/lib:$ORIGIN/lib") );
  }
}

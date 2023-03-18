// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2020-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ExecutableFile/Algorithm.h"
#include <QLatin1String>
#include <QLatin1Char>
#include <QString>
#include <QFileInfo>
#include <vector>
#include <string>


QFileInfo fileInfoFromString(const std::string & file)
{
  return QFileInfo( QString::fromStdString(file) );
}

TEST_CASE("stringStartsWith")
{
  using Mdt::ExecutableFile::stringStartsWith;

  std::string str;

  SECTION("str empty")
  {
    SECTION("s A")
    {
      REQUIRE( !stringStartsWith(str, "A") );
    }
  }

  SECTION("str A")
  {
    str = "A";

    SECTION("s A")
    {
      REQUIRE( stringStartsWith(str, "A") );
    }

    SECTION("s a")
    {
      REQUIRE( !stringStartsWith(str, "a") );
    }

    SECTION("s B")
    {
      REQUIRE( !stringStartsWith(str, "B") );
    }
  }

  SECTION("str AB")
  {
    str = "AB";

    SECTION("s A")
    {
      REQUIRE( stringStartsWith(str, "A") );
    }

    SECTION("s a")
    {
      REQUIRE( !stringStartsWith(str, "a") );
    }

    SECTION("s B")
    {
      REQUIRE( !stringStartsWith(str, "B") );
    }

    SECTION("s AA")
    {
      REQUIRE( !stringStartsWith(str, "AA") );
    }

    SECTION("s AB")
    {
      REQUIRE( stringStartsWith(str, "AB") );
    }

    SECTION("s ABC")
    {
      REQUIRE( !stringStartsWith(str, "ABC") );
    }
  }
}

TEST_CASE("joinToStdString")
{
  using Mdt::ExecutableFile::joinToStdString;

  SECTION("empty")
  {
    REQUIRE( joinToStdString({},';') == "" );
  }

  SECTION("A")
  {
    REQUIRE( joinToStdString({"A"},';') == "A" );
  }

  SECTION("A,B")
  {
    REQUIRE( joinToStdString({"A","B"},';') == "A;B" );
  }

  SECTION("A,B,C")
  {
    REQUIRE( joinToStdString({"A","B","C"},';') == "A;B;C" );
  }
}

TEST_CASE("joinToQString")
{
  using Mdt::ExecutableFile::joinToQString;

  std::vector<std::string> container;
  QString str;

  const auto toQString = [](const std::string & s){
    return QString::fromStdString(s);
  };

  SECTION("empty container")
  {
    str = joinToQString( container, toQString, QLatin1String(", ") );

    REQUIRE( str.isEmpty() );
  }

  SECTION("A")
  {
    container = {"A"};

    str = joinToQString( container, toQString, QLatin1String(", ") );

    REQUIRE( str == QLatin1String("A") );
  }

  SECTION("A, B")
  {
    container = {"A","B"};

    str = joinToQString( container, toQString, QLatin1String(", ") );

    REQUIRE( str == QLatin1String("A, B") );
  }

  SECTION("A, B, C")
  {
    container = {"A","B","C"};

    str = joinToQString( container, toQString, QLatin1String(", ") );

    REQUIRE( str == QLatin1String("A, B, C") );
  }
}

TEST_CASE("appendToStdVector")
{
  using Mdt::ExecutableFile::appendToStdVector;

  std::vector<std::string> destination;
  std::vector<std::string> reference;

  SECTION("empty -> empty")
  {
    appendToStdVector({}, destination);
    reference = {};
    REQUIRE( destination == reference );
  }

  SECTION("A -> empty")
  {
    appendToStdVector({"A"}, destination);
    reference = {"A"};
    REQUIRE( destination == reference );
  }

  SECTION("empty -> A")
  {
    destination = {"A"};
    appendToStdVector({}, destination);
    reference = {"A"};
    REQUIRE( destination == reference );
  }

  SECTION("B -> A")
  {
    destination = {"A"};
    appendToStdVector({"B"}, destination);
    reference = {"A","B"};
    REQUIRE( destination == reference );
  }
}

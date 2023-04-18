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
#include "HeaderTableGraphicsItemMapId.h"
#include <QMetaType>
#include <QString>


TEST_CASE("simpleValue")
{
  const auto id = HeaderTableGraphicsItemMapId::fromValue(25);

  REQUIRE( id.value() == 25 );
}

TEST_CASE("isValidQVariant")
{
  SECTION("Invalid variant")
  {
    const QVariant variant = QVariant();

    REQUIRE( !HeaderTableGraphicsItemMapId::isValidQVariant(variant) );
  }

  SECTION("Null variant")
  {
    /*
     * In Qt5, QVariant::Type is obselete,
     * but the constructor taking QMetaType is deleted.
     * In Qt6, we should use the constructor taking QMetaType.
     */
//     const QVariant variant = QVariant(QMetaType::ULongLong);
    const QVariant variant = QVariant(QVariant::ULongLong);

    REQUIRE( !HeaderTableGraphicsItemMapId::isValidQVariant(variant) );
  }

  SECTION("valid variant with 0")
  {
    qulonglong value = 0;
    const auto variant = QVariant::fromValue(value);

    REQUIRE( HeaderTableGraphicsItemMapId::isValidQVariant(variant) );
  }

  SECTION("variant holding a string")
  {
    const auto variant = QVariant::fromValue( QString::fromLatin1("25") );

    REQUIRE( !HeaderTableGraphicsItemMapId::isValidQVariant(variant) );
  }
}

TEST_CASE("toQVariant")
{
  const auto id = HeaderTableGraphicsItemMapId::fromValue(25);

  const QVariant variant = id.toQVariant();

  REQUIRE( HeaderTableGraphicsItemMapId::isValidQVariant(variant) );
  REQUIRE( variant.toULongLong() == 25 );
}

TEST_CASE("fromQVariant")
{
  const QVariant variant = HeaderTableGraphicsItemMapId::fromValue(34).toQVariant();

  const auto id = HeaderTableGraphicsItemMapId::fromQVariant(variant);

  REQUIRE( id.value() == 34 );
}

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
#include "SectionHeaderTableModel.h"
#include "HeaderTableGraphicsItemMapId.h"
#include <QLatin1String>

using Mdt::ExecutableFile::Elf::SectionHeader;

const int nameColumn = static_cast<int>(SectionHeaderTableModel::Column::Name);
const int offsetColumn = static_cast<int>(SectionHeaderTableModel::Column::Offset);
const int sizeColumn = static_cast<int>(SectionHeaderTableModel::Column::Size);


TEST_CASE("dimensions")
{
  SectionHeaderTableModel model;

  SECTION("empty model")
  {
    REQUIRE( model.columnCount() == 3 );
    REQUIRE( model.rowCount() == 0 );
  }

  SECTION("1 header")
  {
    SectionHeader header;
    const auto id = HeaderTableGraphicsItemMapId::fromValue(1);

    model.addSection(header, id);

    REQUIRE( model.columnCount() == 3 );
    REQUIRE( model.rowCount() == 1 );
  }
}

TEST_CASE("sortRole")
{
  SectionHeaderTableModel model;

  REQUIRE( model.sortRole() > Qt::UserRole );
}

TEST_CASE("data")
{
  SectionHeaderTableModel model;
  const int sortRole = model.sortRole();

  SECTION("1 header")
  {
    SectionHeader header;
    header.name = ".dynstr";
    header.offset = 10;
    header.size = 25;
    const auto id = HeaderTableGraphicsItemMapId::fromValue(1);

    model.addSection(header, id);

    QModelIndex index = model.index(0, nameColumn);
    REQUIRE( model.data(index, Qt::DisplayRole).toString() == QLatin1String(".dynstr") );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );
    REQUIRE( model.data(index, sortRole).toString() == QLatin1String(".dynstr") );

    index = model.index(0, offsetColumn);
    REQUIRE( !model.data(index, Qt::DisplayRole).toString().isEmpty() );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );
    REQUIRE( model.data(index, sortRole).toULongLong() == 10 );

    index = model.index(0, sizeColumn);
    REQUIRE( !model.data(index, Qt::DisplayRole).toString().isEmpty() );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );
    REQUIRE( model.data(index, sortRole).toULongLong() == 25 );
  }
}

TEST_CASE("headerData")
{
  SectionHeaderTableModel model;

  REQUIRE( !model.headerData(0, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(0, Qt::Horizontal ,Qt::UserRole).isNull() );

  REQUIRE( !model.headerData(1, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(1, Qt::Horizontal ,Qt::UserRole).isNull() );

  REQUIRE( !model.headerData(2, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(2, Qt::Horizontal ,Qt::UserRole).isNull() );
}

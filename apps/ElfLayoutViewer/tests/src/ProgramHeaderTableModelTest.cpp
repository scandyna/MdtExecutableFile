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
#include "ProgramHeaderTableModel.h"
#include "HeaderTableGraphicsItemMapId.h"
#include <QLatin1String>

using Mdt::ExecutableFile::Elf::ProgramHeader;
using Mdt::ExecutableFile::Elf::SegmentType;

const int typeColumn = static_cast<int>(ProgramHeaderTableModel::Column::Type);
const int offsetColumn = static_cast<int>(ProgramHeaderTableModel::Column::Offset);
const int sizeColumn = static_cast<int>(ProgramHeaderTableModel::Column::Size);


TEST_CASE("dimensions")
{
  ProgramHeaderTableModel model;

  SECTION("empty model")
  {
    REQUIRE( model.columnCount() == 3 );
    REQUIRE( model.rowCount() == 0 );
  }

  SECTION("1 header")
  {
    ProgramHeader header;
    const auto id = HeaderTableGraphicsItemMapId::fromValue(1);

    model.addSection(header, id);

    REQUIRE( model.columnCount() == 3 );
    REQUIRE( model.rowCount() == 1 );
  }
}

TEST_CASE("data")
{
  ProgramHeaderTableModel model;

  SECTION("1 header")
  {
    ProgramHeader header;
    header.setSegmentType(SegmentType::Load);
    header.offset = 10;
    header.filesz = 25;
    const auto id = HeaderTableGraphicsItemMapId::fromValue(1);

    model.addSection(header, id);

    QModelIndex index = model.index(0, typeColumn);
    REQUIRE( model.data(index, Qt::DisplayRole).toString() == QLatin1String("LOAD") );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );

    index = model.index(0, offsetColumn);
    REQUIRE( !model.data(index, Qt::DisplayRole).toString().isEmpty() );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );

    index = model.index(0, sizeColumn);
    REQUIRE( !model.data(index, Qt::DisplayRole).toString().isEmpty() );
    REQUIRE( model.data(index, Qt::UserRole).toULongLong() == 1 );
  }
}

TEST_CASE("headerData")
{
  ProgramHeaderTableModel model;

  REQUIRE( !model.headerData(0, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(0, Qt::Horizontal ,Qt::UserRole).isNull() );

  REQUIRE( !model.headerData(1, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(1, Qt::Horizontal ,Qt::UserRole).isNull() );

  REQUIRE( !model.headerData(2, Qt::Horizontal ,Qt::DisplayRole).toString().isEmpty() );
  REQUIRE( model.headerData(2, Qt::Horizontal ,Qt::UserRole).isNull() );
}

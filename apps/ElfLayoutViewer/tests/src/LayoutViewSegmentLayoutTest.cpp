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
#include "LayoutViewSegmentLayout.h"
#include <cassert>

using Catch::Matchers::WithinRel;
using Mdt::ExecutableFile::Elf::ProgramHeader;

// Note that last is inbound (NOT past)
LayoutViewSegmentLayoutItem makeItemWithFirstAndLastAddressAndHeight(uint64_t first, uint64_t last, qreal height)
{
  assert( first <= last );

  ProgramHeader header;
  header.offset = first;
  header.filesz = last + 1;

  return LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, height);
}

// Note that last is inbound (NOT past)
LayoutViewSegmentLayoutItem makeItemWithFirstAndLastAddress(uint64_t first, uint64_t last)
{
  assert( first <= last );

  return makeItemWithFirstAndLastAddressAndHeight(first, last, 1.0);
}


TEST_CASE("pos")
{
  LayoutViewSegmentLayout layout;
  QPointF pos(1.2, 3.4);

  layout.setPos(pos);

  REQUIRE( layout.pos() == pos );
}

TEST_CASE("isEmpty")
{
  LayoutViewSegmentLayout layout;

  SECTION("by default the layout is empty")
  {
    REQUIRE( layout.isEmpty() );
  }

  SECTION("after adding a item the layout is not empty")
  {
    ProgramHeader header;
    header.offset = 100;
    header.filesz = 10;
    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    layout.findPositionAndAddItem(item);

    REQUIRE( !layout.isEmpty() );
  }
}

// TEST_CASE("makePositionForItemAndRowNumber")
// {
//   ProgramHeader header;
//   header.offset = 1;
//   header.filesz = 2;
//
//   LayoutViewSegmentLayout layout;
//   layout.setPos( QPointF(10.0, 0.0) );
//
//   SECTION("Row 0 (height 30)")
//   {
//     const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 30.0);
//
//     const QPointF pos = layout.makePositionForItemAndRowNumber(item, 0);
//
//     REQUIRE_THAT( pos.x(), WithinRel(1.0) );
//     REQUIRE_THAT( pos.y(), WithinRel(10.0) );
//   }
//
//   /// todo difficult to test if we can't add a row
//   SECTION("Row 0 has height 50 now row 1 (height 30)")
//   {
//     /// todo add a item with height 50 at row 0
//     const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 30.0);
//
//     const QPointF pos = layout.makePositionForItemAndRowNumber(item, 1);
//
//     REQUIRE_THAT( pos.x(), WithinRel(1.0) );
//     REQUIRE_THAT( pos.y(), WithinRel(10.0 + 50.0) );
//   }
// }

TEST_CASE("itemsOverlapsInXaxis")
{
  /*
   * |  a |  b   |
   * |0  9|10  15|
   */
  SECTION("a(0 to 9) not overlaps b(10 to 15)")
  {
    const auto a = makeItemWithFirstAndLastAddress(0, 9);
    const auto b = makeItemWithFirstAndLastAddress(10, 15);

    REQUIRE( !LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |  a |
   * |0  9| b
   *     |9  10|
   */
  SECTION("a(0-9) overlaps b(9-10)")
  {
    const auto a = makeItemWithFirstAndLastAddress(0, 9);
    const auto b = makeItemWithFirstAndLastAddress(9, 10);

    REQUIRE( LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |a|  b   |
   * |0|1   15|
   */
  SECTION("a(0 to 0) not overlaps b(1 to 15)")
  {
    const auto a = makeItemWithFirstAndLastAddress(0, 0);
    const auto b = makeItemWithFirstAndLastAddress(1, 15);

    REQUIRE( !LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |  a  |
   * |1  10|
   *
   *  | b |
   *  |2 9|
   */
  SECTION("a(0 to 10) overlaps b(2 to 9)")
  {
    const auto a = makeItemWithFirstAndLastAddress(1, 10);
    const auto b = makeItemWithFirstAndLastAddress(2, 9);

    REQUIRE( LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |  b  |
   * |1  10|
   *
   *  | a |
   *  |2 9|
   */
  SECTION("b(0 to 10) overlaps a(2 to 9)")
  {
    const auto b = makeItemWithFirstAndLastAddress(1, 10);
    const auto a = makeItemWithFirstAndLastAddress(2, 9);

    REQUIRE( LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |  b |  a   |
   * |0  9|10  15|
   */
  SECTION("b(0 to 9) not overlaps a(10 to 15)")
  {
    const auto b = makeItemWithFirstAndLastAddress(0, 9);
    const auto a = makeItemWithFirstAndLastAddress(10, 15);

    REQUIRE( !LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }

  /*
   * |  b |
   * |0  9| a
   *     |9  10|
   */
  SECTION("b(0-9) overlaps a(9-10)")
  {
    const auto b = makeItemWithFirstAndLastAddress(0, 9);
    const auto a = makeItemWithFirstAndLastAddress(9, 10);

    REQUIRE( LayoutViewSegmentLayout::itemsOverlapsInXaxis(a, b) );
  }
}

TEST_CASE("rowCanHoldItem")
{
  using Row = std::vector<LayoutViewSegmentLayoutItem>;

  Row row;
  LayoutViewSegmentLayout layout;
  ProgramHeader header;

  layout.setPos( QPointF(0.0, 0.0) );

  SECTION("empty row")
  {
    header.offset = 100;
    header.filesz = 10;
    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( layout.rowCanHoldItem(row, item) );
  }

  /* |existing|
   * |100  109|110  119|
   */
  SECTION("row with 1 item can accept new item just at the end")
  {
    header.offset = 100;
    header.filesz = 10;
    row.push_back( LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0) );

    header.offset = 110;
    header.filesz = 10;
    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( layout.rowCanHoldItem(row, item) );
  }

  /* |existing|
   * |100  109|
   *         |109  118|
   */
  SECTION("row with 1 item cannot accept a item that overlaps")
  {
    header.offset = 100;
    header.filesz = 10;
    row.push_back( LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0) );

    header.offset = 109;
    header.filesz = 10;
    const auto item = LayoutViewSegmentLayoutItem::fromHeaderAndHeight(header, 20.0);

    REQUIRE( !layout.rowCanHoldItem(row, item) );
  }
}

TEST_CASE("findPositionAndAddItem")
{
  LayoutViewSegmentLayout layout;
  layout.setPos( QPointF(10.0, 20.0) );

  LayoutViewSegmentLayoutItem item;
  QPointF pos;

  /*
   * |0  15|
   */
  item = makeItemWithFirstAndLastAddressAndHeight(0, 15, 30.0);
  pos = layout.findPositionAndAddItem(item);
  REQUIRE_THAT( pos.x(), WithinRel(10.0) );
  REQUIRE_THAT( pos.y(), WithinRel(20.0) );

  /*
   * |0  15|
   *   |5  25|
   */
  item = makeItemWithFirstAndLastAddressAndHeight(5, 25, 40.0);
  pos = layout.findPositionAndAddItem(item);
  REQUIRE_THAT( pos.x(), WithinRel(10.0 + 5.0) );
  REQUIRE_THAT( pos.y(), WithinRel(20.0 + 30.0) );

  /*
   * |0  15|
   *   |5  25|
   *    |6  30|
   */
  item = makeItemWithFirstAndLastAddressAndHeight(6, 30, 35.0);
  pos = layout.findPositionAndAddItem(item);
  REQUIRE_THAT( pos.x(), WithinRel(10.0 + 6.0) );
  REQUIRE_THAT( pos.y(), WithinRel(20.0 + 30.0 + 40.0) );
}

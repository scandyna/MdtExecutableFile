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
#include "HeaderTableGraphicsItemMap.h"
#include "LayoutGraphicsItem.h"
#include <memory>


std::unique_ptr<LayoutGraphicsItem> makeItem()
{
  return std::make_unique<LayoutGraphicsItem>();
}


TEST_CASE("registerItem")
{
  HeaderTableGraphicsItemMap map;

  const auto item0 = makeItem();
  const auto id0 = map.registerItem( item0.get() );
  REQUIRE( id0.value() == 0 );

  const auto item1 = makeItem();
  const auto id1 = map.registerItem( item1.get() );
  REQUIRE( id1.value() == 1 );
}

TEST_CASE("containsId")
{
  HeaderTableGraphicsItemMap map;

  const auto item0 = makeItem();
  const auto id0 = map.registerItem( item0.get() );

  const auto item1 = makeItem();
  const auto id1 = map.registerItem( item1.get() );

  const auto id2 = HeaderTableGraphicsItemMapId::fromValue(2);

  REQUIRE( map.containsId(id0) );
  REQUIRE( map.containsId(id1) );
  REQUIRE( !map.containsId(id2) );
}

TEST_CASE("itemForId")
{
  HeaderTableGraphicsItemMap map;

  const auto item0 = makeItem();
  const auto id0 = map.registerItem( item0.get() );

  REQUIRE( map.itemForId(id0) == item0.get() );
}

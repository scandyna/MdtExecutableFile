// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "HeaderTableGraphicsItemMap.h"
#include <cassert>

HeaderTableGraphicsItemMapId HeaderTableGraphicsItemMap::registerItem(LayoutGraphicsItem *item) noexcept
{
  assert( item != nullptr);

  mList.push_back(item);

  return HeaderTableGraphicsItemMapId::fromValue( mList.size() - 1 );
}

bool HeaderTableGraphicsItemMap::containsId(HeaderTableGraphicsItemMapId id) const noexcept
{
  return id.value() < mList.size();
}

LayoutGraphicsItem *HeaderTableGraphicsItemMap::itemForId(HeaderTableGraphicsItemMapId id) const noexcept
{
  assert( containsId(id) );

  return mList[ id.value() ];
}

void HeaderTableGraphicsItemMap::clear() noexcept
{
  mList.clear();
}

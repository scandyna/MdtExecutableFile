// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "LayoutViewGraphicsScene.h"
#include "SectionGraphicsItemData.h"
#include "SegmentGraphicsItemData.h"
#include <algorithm>
#include <cassert>


SectionGraphicsItem *LayoutViewGraphicsScene::addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header) noexcept
{
  auto item = new SectionGraphicsItem( SectionGraphicsItemData::fromSectionHeader(header) );

  updateSectionsAreaHeight(*item);

  mScene.addItem(item);

  return item;
}

SegmentGraphicsItem *LayoutViewGraphicsScene::addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept
{
  if( mSegmentLayout.isEmpty() ){
    mSegmentLayout.setPos( QPointF(0.0, mSectionsAreaHeight + 30.0) );
  }

  auto item = new SegmentGraphicsItem( SegmentGraphicsItemData::fromProgramHeader(header) );

  const auto layoutItem = LayoutViewSegmentLayoutItem::fromHeaderAndHeight( header, item->height() );
  const QPointF itemPos = mSegmentLayout.findPositionAndAddItem(layoutItem);

  item->setPos(itemPos);

  mScene.addItem(item);

  return item;
}

void LayoutViewGraphicsScene::updateSectionsAreaHeight(const SectionGraphicsItem & item) noexcept
{
  const qreal itemHeight = item.height();

  mSectionsAreaHeight = std::max(mSectionsAreaHeight, itemHeight);
}

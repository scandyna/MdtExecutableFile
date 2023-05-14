// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "LayoutViewSegmentLayout.h"
#include "Numeric.h"
#include <algorithm>
#include <cassert>

qreal LayoutViewSegmentLayoutItem::offsetF() const noexcept
{
  return qreal_from_uint64(mOffset);
}

void LayoutViewSegmentLayout::setPos(QPointF pos) noexcept
{
  assert( isEmpty() );

  mPos = pos; 
}

void LayoutViewSegmentLayout::clear() noexcept
{
  mMatrix.clear();
}

QPointF LayoutViewSegmentLayout::findPositionAndAddItem(const LayoutViewSegmentLayoutItem & item) noexcept
{
  std::size_t rowNumber = 0;

  for(; rowNumber < mMatrix.size(); ++rowNumber){
    if( rowCanHoldItem(mMatrix[rowNumber], item) ){
      mMatrix[rowNumber].push_back(item);
      return makePositionForItemAndRowNumber(item, rowNumber);
    }
  }

  std::vector<LayoutViewSegmentLayoutItem> newRow;
  newRow.push_back(item);
  mMatrix.push_back(newRow);

  return makePositionForItemAndRowNumber(item, rowNumber);
}

qreal LayoutViewSegmentLayout::findRowHeightForRowNumber(std::size_t rowNumber) const noexcept
{
  assert( rowNumber < mMatrix.size() );

  const auto comp = [](const LayoutViewSegmentLayoutItem & a, const LayoutViewSegmentLayoutItem & b){
    return a.heightF() < b.heightF();
  };

  const auto rowEnd = mMatrix[rowNumber].cend();
  const auto it = std::max_element(mMatrix[rowNumber].cbegin(), rowEnd, comp);

  if(it == rowEnd){
    return 0.0;
  }

  return it->heightF();
}

QPointF LayoutViewSegmentLayout::makePositionForItemAndRowNumber(const LayoutViewSegmentLayoutItem & item,
                                                                 std::size_t rowNumber) const noexcept
{
  assert( rowNumber < mMatrix.size() );

  const qreal x = mPos.x() + item.offsetF();

  qreal y = mPos.y();

  for(std::size_t i = 0; i < rowNumber; ++i){
    y += findRowHeightForRowNumber(i);
  }

  return QPointF(x, y);
}

bool LayoutViewSegmentLayout::itemsOverlapsInXaxis(const LayoutViewSegmentLayoutItem & a,
                                                   const LayoutViewSegmentLayoutItem & b) noexcept
{
  /*
   *        a               b
   * |offset lastAddr|offset lastAddr|
   */
  if( a.lastAddress() < b.offset() ){
    return false;
  }

  /*
   *        b               a
   * |offset lastAddr|offset lastAddr|
   */
  if( b.lastAddress() < a.offset() ){
    return false;
  }

  return true;
}

bool LayoutViewSegmentLayout::rowCanHoldItem(const std::vector<LayoutViewSegmentLayoutItem> & row,
                                             const LayoutViewSegmentLayoutItem & item) noexcept
{
  for(const LayoutViewSegmentLayoutItem & currentItem : row){
    if( itemsOverlapsInXaxis(currentItem, item) ){
      return false;
    }
  }

  return true;
}

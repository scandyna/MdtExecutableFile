// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LAYOUT_VIEW_SEGMENT_LAYOUT_H
#define LAYOUT_VIEW_SEGMENT_LAYOUT_H

#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <QPointF>
#include <QRectF>
#include <vector>
#include <cstdint>

/*! \brief Item data for LayoutViewSegmentLayout
 */
class LayoutViewSegmentLayoutItem
{
 public:

  /*! \brief Get the offset of this item
   *
   * \sa offsetF()
   */
  uint64_t offset() const noexcept
  {
    return mOffset;
  }

  /*! \brief Get the offset of this item as real
   *
   * \sa offset()
   */
  qreal offsetF() const noexcept;

  /*! \brief Get the last address
   *
   * \note for a empty section, the last address is equal to the offset.
   *  This is different from the STL end concept.
   */
  uint64_t lastAddress() const noexcept
  {
    return mLastAddress;
  }

  /*! \brief Get the height of this item
   */
  qreal heightF() const noexcept
  {
    return mHeight;
  }

  /*! \brief Get a item for given header and height
   */
  static
  LayoutViewSegmentLayoutItem fromHeaderAndHeight(const Mdt::ExecutableFile::Elf::ProgramHeader & header, qreal height) noexcept
  {
    LayoutViewSegmentLayoutItem item;

    item.mOffset = header.offset;
    const uint64_t size = header.filesz;
    if(size == 0){
      item.mLastAddress = header.offset;
    }else{
      item.mLastAddress = header.offset + size - 1;
    }
    item.mHeight = height;

    return item;
  }

 private:

  uint64_t mOffset = 0;
  uint64_t mLastAddress = 0;
  qreal mHeight = 0.0;
};

/*! \brief Helper to provide a layout for segments in the LayoutViewGraphicsScene
 *
 * In an ELF file, segments can overlap.
 * As an example, a LOAD segment can cover other segments.
 *
 * To be able to display the segments in a somewhat compact way,
 * we try to add non overlapping segments to the same row.
 *
 * To achieve this, we introduce a concept of rows and columns.
 * This is not a real grid, because columns can have different sizes
 * between rows. Also, the count of colums differs between rows.
 *
 * \note Its tempting to consider this class as a QGraphicsItemGroup,
 * but its not.
 * This is a helper to position items to a scene.
 * Also, moving items has not to be supported
 * because we do not support editing a ELF file.
 */
class LayoutViewSegmentLayout
{
 public:

  /*! \brief Set the position of this layout in the scene
   *
   * \pre This layout must be empty
   */
  void setPos(QPointF pos) noexcept;

  /*! \brief Get the position of this layout in the scene
   */
  QPointF pos() const noexcept
  {
    return mPos;
  }

  /*! \brief Check if this layout is empty
   *
   * Returns true if this layout contains no items
   */
  bool isEmpty() const noexcept
  {
    return mMatrix.empty();
  }

  /*! \brief Clear this layout
   */
  void clear() noexcept;

  /*! \brief Find a position for given item in this layout and add it
   *
   * \todo Algorithm complexity is bad (see implementation)
   */
  QPointF findPositionAndAddItem(const LayoutViewSegmentLayoutItem & item) noexcept;

  /*! \internal Find the row height for given row number
   *
   * \pre \a rowNumber must be in range: 0 <= \a rowNumber < matrix row count
   */
  qreal findRowHeightForRowNumber(std::size_t rowNumber) const noexcept;

  /*! \internal Make a position for given item and row number
   *
   * \pre \a rowNumber must be in range: 0 <= \a rowNumber < matrix row count
   */
  QPointF makePositionForItemAndRowNumber(const LayoutViewSegmentLayoutItem & item, std::size_t rowNumber) const noexcept;

  /*! \internal Check if given items overlap each other
   *
   * \note this only checks for the x axis
   */
  static
  bool itemsOverlapsInXaxis(const LayoutViewSegmentLayoutItem & a, const LayoutViewSegmentLayoutItem & b) noexcept;

  /*! \internal Check if given row can hold given item
   */
  static
  bool rowCanHoldItem(const std::vector<LayoutViewSegmentLayoutItem> & row,
                      const LayoutViewSegmentLayoutItem & item) noexcept;

 private:

  QPointF mPos;
  std::vector< std::vector<LayoutViewSegmentLayoutItem> > mMatrix;
};

#endif // #ifndef LAYOUT_VIEW_SEGMENT_LAYOUT_H

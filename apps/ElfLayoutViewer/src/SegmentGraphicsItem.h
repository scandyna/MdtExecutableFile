// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SEGMENT_GRAPHICS_ITEM_H
#define SEGMENT_GRAPHICS_ITEM_H

#include "SegmentGraphicsItemData.h"
#include "LayoutGraphicsItem.h"
#include <QString>


/*! \brief Represents a ELF segment as a Qt graphics item
 */
class SegmentGraphicsItem : public LayoutGraphicsItem
{
 public:

  /*! \brief Constructor
   */
  explicit SegmentGraphicsItem(const SegmentGraphicsItemData & data, QGraphicsItem *parent = nullptr);
};

#endif // #ifndef SEGMENT_GRAPHICS_ITEM_H

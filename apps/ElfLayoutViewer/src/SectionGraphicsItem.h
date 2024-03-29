// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SECTION_GRAPHICS_ITEM_H
#define SECTION_GRAPHICS_ITEM_H

#include "SectionGraphicsItemData.h"
#include "LayoutGraphicsItem.h"


/*! \brief Represents a ELF section as a Qt graphics item
 */
class SectionGraphicsItem : public LayoutGraphicsItem
{
 public:

  /*! \brief Constructor
   */
  explicit SectionGraphicsItem(const SectionGraphicsItemData & data, QGraphicsItem *parent = nullptr);
};

#endif // #ifndef SECTION_GRAPHICS_ITEM_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LAYOUT_GRAPHICS_ITEM_H
#define LAYOUT_GRAPHICS_ITEM_H

#include <QGraphicsItemGroup>
#include <QSizeF>
#include <QString>

/*! \brief Common base for SectionGraphicsItem and SegmentGraphicsItem
 */
class LayoutGraphicsItem : public QGraphicsItemGroup
{
 public:

  /*! \brief Constructor
   */
  explicit LayoutGraphicsItem(QGraphicsItem *parent = nullptr);

 protected:

  void createRectangle(const QSizeF & size) noexcept;
  void createLabel(const QString & text) noexcept;

  void createStartAddressLabel(const QString & text) noexcept;
  void createNameAndSizeLabel(const QString & text) noexcept;
  void createEndAddressLabel(const QString & text) noexcept;
};

#endif // #ifndef LAYOUT_GRAPHICS_ITEM_H

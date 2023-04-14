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

#include "LayoutGraphicsItem.h"
#include <QString>

#include <QtGlobal>

#include <cstdint>
#include <string>

/*! \brief
 *
 * \todo give a correct name, make correct API !
 */
class SegmentGraphicsItemData
{
 public:

  void setOffset(uint64_t offset) noexcept
  {
    mOffset = offset;
  }

  qulonglong offset() const noexcept
  {
    return mOffset;
  }

  qreal offsetF() const noexcept
  {
    return mOffset;
  }

  void setSize(uint64_t size) noexcept
  {
    mSize = size;
  }

  qulonglong size() const noexcept
  {
    return mSize;
  }

  qreal sizeF() const noexcept
  {
    return mSize;
  }

  /*! \brief
   *
   * \todo rename to something like last address.
   * Then, offset + size - 1
   * Also handle the 0 size case
   *
   * Also document the choice of not using end, because its confusing with STL terminology (pas last element)
   */
  qulonglong end() const noexcept
  {
    return mOffset + mSize;
  }

  void setName(const std::string & name) noexcept
  {
    mName = QString::fromStdString(name);
  }

  const QString & name() const noexcept
  {
    return mName;
  }

  /// \todo check, think a segment can have offset and size 0 ?
  bool isNull() const noexcept
  {
  }

  static
  SegmentGraphicsItemData fromProgramHeader() noexcept;

 private:

  qulonglong mOffset = 0;
  qulonglong mSize = 0;
  QString mName;
};

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

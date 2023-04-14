// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SegmentGraphicsItem.h"
#include <QSizeF>
#include <QPointF>

SegmentGraphicsItem::SegmentGraphicsItem(const SegmentGraphicsItemData & data, QGraphicsItem *parent)
 : LayoutGraphicsItem(parent)
{
  const qulonglong sectionOffset = data.offset();
  const qulonglong sectionEnd = data.end();

  QSizeF rectSize(data.sizeF(), 20.0);

  createRectangle(rectSize);

  /// \todo should we inherit also QObject to use tr() ?

  const auto text = QString( QLatin1String("%1\nstart: 0x%2 (%3)\nsize: %4 (0x%5)\nend: 0x%6 (%7)") )
                    .arg( data.name() )
                    .arg(sectionOffset, 0, 16)
                    .arg(sectionOffset)
                    .arg( data.size() )
                    .arg( data.size(), 0, 16)
                    .arg(sectionEnd, 0, 16)
                    .arg(sectionEnd);
  createLabel(text);

  QPointF itemPos(data.offsetF(), 0.0);
  setPos(itemPos);
}

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SectionGraphicsItem.h"
#include <QPointF>
#include <QSizeF>
#include <QPainterPath>
#include <QString>
#include <QLatin1Char>
#include <QLatin1String>


SectionGraphicsItem::SectionGraphicsItem(const SectionGraphicsItemData & data, QGraphicsItem *parent)
 : LayoutGraphicsItem(parent)
{
  const qulonglong sectionOffset = data.offset();
  const qulonglong sectionEnd = data.lastAddress();

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

//   const auto startAddrText = QString( QLatin1String("start: 0x%1 (%2)") )
//                              .arg( sectionOffset, 0, 16, QLatin1Char('0') )
//                              .arg(sectionOffset);
//   createStartAddressLabel(startAddrText);
//
//   const auto centerText = QString( QLatin1String("%1\nsize: %2 (0x%3)") )
//                           .arg( data.name() )
//                           .arg( data.size() )
//                           .arg( data.size(), 0, 16 );
//   createNameAndSizeLabel(centerText);
//
//   const auto endAddrText = QString( QLatin1String("end: 0x%1 (%2)") )
//                            .arg( sectionEnd, 0, 16, QLatin1Char('0') )
//                            .arg(sectionEnd);
//   createEndAddressLabel(endAddrText);

  QPointF itemPos(data.offsetF(), 0.0);
  setPos(itemPos);
}

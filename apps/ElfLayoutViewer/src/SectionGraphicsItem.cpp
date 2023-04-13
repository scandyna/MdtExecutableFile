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
#include <QRectF>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPainterPath>
#include <QString>
#include <QLatin1Char>

#include <cstdint>

#include <QLatin1String>

#include <QDebug>

SectionGraphicsItem::SectionGraphicsItem(const SectionGraphicsItemData & data, QGraphicsItem *parent)
 : LayoutGraphicsItem(parent)
{
  const qulonglong sectionOffset = data.offset();
  const qulonglong sectionEnd = data.end();
//   uint64_t sectionSize = data.size;
//   uint64_t sectionEnd = sectionOffset + sectionSize;
//   uint64_t sectionEnd = data.offset + data.size;
  
//   QPointF rectTopLeft(sectionOffset, 0.0);

//   QPointF rectTopLeft(0.0, 0.0);
  QSizeF rectSize(data.sizeF(), 30.0);
//   QRectF rect(rectTopLeft, rectSize);
//   auto rectItem = new QGraphicsRectItem(rect);
//   addToGroup(rectItem);

  createRectangle(rectSize);

  qDebug() << "item shape: " << shape();
  qDebug() << "item shape boundingRect: " << shape().boundingRect();
  qDebug() << "item boundingRect: " << boundingRect();
  
  
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
// //   QPointF startAddrLabelPos(sectionOffset, 0.0);
// //   auto startAddrLabel = new QGraphicsSimpleTextItem(startAddrText);
// //   startAddrLabel->setPos(startAddrLabelPos);
// //   addToGroup(startAddrLabel);
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

  /** \todo Should not position itself !
   *
   * Why, sections will be aligned one after the other,
   * segments one after but also below
   */
  QPointF itemPos(data.offsetF(), 0.0);
  setPos(itemPos);
//   auto endAddrLabel = new QGraphicsSimpleTextItem(endAddrText);
//   
//   const QPainterPath painterPath = endAddrLabel->shape();
//   qDebug() << painterPath;
//   qDebug() << "len: " << painterPath.length();
//   
//   qDebug() << "boundingRect: " << painterPath.boundingRect();
//   qDebug() << "controlPointRect: " << painterPath.controlPointRect();
//   
//   ///const qreal endAddrLabelPosTopLeft = sectionEnd - painterPath.length();
//   const qreal endAddrLabelPosTopLeft = sectionEnd - painterPath.boundingRect().width();
//   qDebug() << "x: " << endAddrLabelPosTopLeft;
//   
//   QPointF endAddrLabelPos(endAddrLabelPosTopLeft, 0.0);
//   
//   endAddrLabel->setPos(endAddrLabelPos);
//   addToGroup(endAddrLabel);
}

/// \todo should rename start/end Address and name labels

// void SectionGraphicsItem::createLeftLabel(const QString & text) noexcept
// {
//   auto label = new QGraphicsSimpleTextItem(text);
//   QPointF pos(0.0, 0.0);
//   label->setPos(pos);
//   addToGroup(label);
// }

// void SectionGraphicsItem::createCenterLabel(const QString & text) noexcept
// {
//   auto label = new QGraphicsSimpleTextItem(text);
//   const qreal itemCenter = boundingRect().width() / 2.0;
//   const qreal x = itemCenter - label->boundingRect().width() / 2.0;
//   QPointF pos(x, 0.0);
//   label->setPos(pos);
//   addToGroup(label);
// }

// void SectionGraphicsItem::createRightLabel(const QString & text) noexcept
// {
//   auto label = new QGraphicsSimpleTextItem(text);
//   const qreal x = boundingRect().width() - label->boundingRect().width();
//   QPointF pos(x, 0.0);
//   label->setPos(pos);
//   addToGroup(label);
// }

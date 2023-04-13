// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "LayoutGraphicsItem.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <QPointF>

LayoutGraphicsItem::LayoutGraphicsItem(QGraphicsItem *parent)
 : QGraphicsItemGroup(parent)
{
}

void LayoutGraphicsItem::createRectangle(const QSizeF & size) noexcept
{
  QPointF rectTopLeft(0.0, 0.0);
  QRectF rect(rectTopLeft, size);
  auto rectItem = new QGraphicsRectItem(rect);
  addToGroup(rectItem);
}

/// \todo assert rect was created before !

void LayoutGraphicsItem::createLabel(const QString & text) noexcept
{
  auto label = new QGraphicsSimpleTextItem(text);
  const qreal y = -label->boundingRect().bottom();
  QPointF pos(0.0,y);
  label->setPos(pos);
  label->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
  addToGroup(label);
}

void LayoutGraphicsItem::createStartAddressLabel(const QString & text) noexcept
{
  auto label = new QGraphicsSimpleTextItem(text);
  QPointF pos(0.0, 0.0);
  label->setPos(pos);
  label->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
  addToGroup(label);
}

/// \todo we should only use boundingRect() on text labels, for others, we know the width
/// \todo also abstract getting width of label

void LayoutGraphicsItem::createNameAndSizeLabel(const QString & text) noexcept
{
  auto label = new QGraphicsSimpleTextItem(text);
  const qreal itemCenter = boundingRect().width() / 2.0;
  const qreal x = itemCenter - label->boundingRect().width() / 2.0;
  QPointF pos(x, 0.0);
  label->setPos(pos);
  label->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
  addToGroup(label);
}

void LayoutGraphicsItem::createEndAddressLabel(const QString & text) noexcept
{
  auto label = new QGraphicsSimpleTextItem(text);
  const qreal x = boundingRect().width() - label->boundingRect().width();
  QPointF pos(x, 0.0);
  label->setPos(pos);
  label->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
  addToGroup(label);
}

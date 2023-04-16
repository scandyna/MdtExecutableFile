// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SEGMENT_GRAPHICS_ITEM_TEST_H
#define SEGMENT_GRAPHICS_ITEM_TEST_H

#include <QTest>
#include <QObject>

class SegmentGraphicsItemTest : public QObject
{
  Q_OBJECT

 private slots:

  void construct();
};

#endif // #ifndef SEGMENT_GRAPHICS_ITEM_TEST_H

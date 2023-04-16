// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SegmentGraphicsItemTest.h"
#include "SegmentGraphicsItem.h"
#include "SegmentGraphicsItemData.h"
#include <QLatin1String>


void SegmentGraphicsItemTest::construct()
{
  SegmentGraphicsItemData data;
  data.setOffset(100);
  data.setSize(50);
  data.setName("LOAD");

  SegmentGraphicsItem item(data);

  QCOMPARE( item.x(), 100.0 );
}

QTEST_MAIN(SegmentGraphicsItemTest)

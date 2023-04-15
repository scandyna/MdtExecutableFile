// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SectionGraphicsItemTest.h"
#include "SectionGraphicsItem.h"
#include "SectionGraphicsItemData.h"
#include <QLatin1String>


void SectionGraphicsItemTest::construct()
{
  SectionGraphicsItemData data;
  data.setOffset(100);
  data.setSize(50);
  data.setName(".dynstr");

  SectionGraphicsItem item(data);

  QCOMPARE( item.x(), 100.0 );
}

QTEST_MAIN(SectionGraphicsItemTest)

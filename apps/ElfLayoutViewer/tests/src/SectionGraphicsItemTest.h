// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SECTION_GRAPHICS_ITEM_TEST_H
#define SECTION_GRAPHICS_ITEM_TEST_H

#include <QTest>
#include <QObject>

class SectionGraphicsItemTest : public QObject
{
  Q_OBJECT

 private slots:

  void construct();
};

#endif // #ifndef SECTION_GRAPHICS_ITEM_TEST_H

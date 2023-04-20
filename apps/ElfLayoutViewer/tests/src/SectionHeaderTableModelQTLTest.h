// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SECTION_HEADER_TABLE_MODEL_QTL_TEST_H
#define SECTION_HEADER_TABLE_MODEL_QTL_TEST_H

#include <QTest>
#include <QObject>

class SectionHeaderTableModelTest : public QObject
{
  Q_OBJECT

 private slots:

  void itemModelTester();
};

#endif // #ifndef SECTION_HEADER_TABLE_MODEL_QTL_TEST_H

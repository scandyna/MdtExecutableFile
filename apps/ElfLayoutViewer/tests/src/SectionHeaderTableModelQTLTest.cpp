// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SectionHeaderTableModelQTLTest.h"
#include "SectionHeaderTableModel.h"
#include <QAbstractItemModelTester>

void SectionHeaderTableModelTest::itemModelTester()
{
  SectionHeaderTableModel model;

  QAbstractItemModelTester tester(&model);
}

QTEST_GUILESS_MAIN(SectionHeaderTableModelTest)

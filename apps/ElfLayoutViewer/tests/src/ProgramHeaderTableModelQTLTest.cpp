// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ProgramHeaderTableModelQTLTest.h"
#include "ProgramHeaderTableModel.h"
#include <QAbstractItemModelTester>

void ProgramHeaderTableModelTest::itemModelTester()
{
  ProgramHeaderTableModel model;

  QAbstractItemModelTester tester(&model);
}

QTEST_GUILESS_MAIN(ProgramHeaderTableModelTest)

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef PROGRAM_HEADER_TABLE_MODEL_QTL_TEST_H
#define PROGRAM_HEADER_TABLE_MODEL_QTL_TEST_H

#include <QTest>
#include <QObject>

class ProgramHeaderTableModelTest : public QObject
{
  Q_OBJECT

 private slots:

  void itemModelTester();
};

#endif // #ifndef PROGRAM_HEADER_TABLE_MODEL_QTL_TEST_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "TestSharedLibrary.h"
#include <QString>
#include <QDebug>

void sayHello()
{
  qDebug() << "Hello";
}

int process(const char *str)
{
  const QString s = QString::fromLocal8Bit(str);

  qDebug() << "process() - str: " << s;

  if( !s.isEmpty() ){
    return 42;
  }

  return 76;
}

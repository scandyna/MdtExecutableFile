// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Catch2QString.h"
#include <QByteArray>

std::ostream & operator <<(std::ostream & os, const QString & str)
{
  os << str.toLocal8Bit().toStdString();

  return os;
}

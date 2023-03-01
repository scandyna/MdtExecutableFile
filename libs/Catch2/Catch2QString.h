// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_CATCH2_QSTRING_H
#define MDT_CATCH2_QSTRING_H

#include <QString>
#include <ostream>

std::ostream & operator <<(std::ostream & os, const QString & str);

#endif // #ifndef MDT_CATCH2_QSTRING_H

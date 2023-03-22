// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef TEST_LIBRARY_H
#define TEST_LIBRARY_H

#include "testsharedlibrary_export.h"

void sayHello();

TESTSHAREDLIBRARY_EXPORT
int process(const char *str);

#endif // #ifndef TEST_LIBRARY_H

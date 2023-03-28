// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2020-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "Mdt/ExecutableFile/Platform.h"
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QTemporaryDir>
#include <vector>
#include <string>
#include <algorithm>

Mdt::ExecutableFile::Platform getNonNativePlatform();

QStringList qStringListFromUtf8Strings(const std::vector<std::string> & args);

QString generateStringWithNChars(int n);

QStringList getTestPrefixPath(const char * const prefixPath);

#endif // #ifndef TEST_UTILS_H

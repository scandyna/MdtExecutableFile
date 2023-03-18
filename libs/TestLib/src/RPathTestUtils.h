// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef RPATH_TEST_UTILS_H
#define RPATH_TEST_UTILS_H

#include "Mdt/ExecutableFile/RPath.h"
#include <string>
#include <vector>

Mdt::ExecutableFile::RPath
makeRPathFromPathList(const std::vector<std::string> & pathList);

#endif // #ifndef RPATH_TEST_UTILS_H

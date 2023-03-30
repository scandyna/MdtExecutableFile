// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
// #include "TestUtils.h"
// #include "TestFileUtils.h"
#include "Mdt/ExecutableFile/ExecutableFileWriter.h"
#include <QString>
#include <QTemporaryFile>

using namespace Mdt::ExecutableFile;

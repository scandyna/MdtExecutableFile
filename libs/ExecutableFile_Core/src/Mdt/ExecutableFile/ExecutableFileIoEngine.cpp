// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ExecutableFileIoEngine.h"
#include "AbstractExecutableFileIoEngine.h"

namespace Mdt{ namespace ExecutableFile{

ExecutableFileIoEngine::ExecutableFileIoEngine(QObject *parent)
 : QObject(parent)
{
}

ExecutableFileIoEngine::~ExecutableFileIoEngine() noexcept
{
}

}} // namespace Mdt{ namespace ExecutableFile{

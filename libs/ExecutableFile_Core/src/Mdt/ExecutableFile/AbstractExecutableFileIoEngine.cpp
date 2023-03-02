// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "AbstractExecutableFileIoEngine.h"

namespace Mdt{ namespace ExecutableFile{

AbstractExecutableFileIoEngine::AbstractExecutableFileIoEngine(QObject* parent)
 : QObject(parent)
{
}

}} // namespace Mdt{ namespace ExecutableFile{

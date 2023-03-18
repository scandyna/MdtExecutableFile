// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RPathTestUtils.h"

using namespace Mdt::ExecutableFile;

RPath makeRPathFromPathList(const std::vector<std::string> & pathList)
{
  RPath rpath;

  for(const std::string & path : pathList){
    rpath.appendPath( QString::fromStdString(path) );
  }

  return rpath;
}

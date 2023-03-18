// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RPath.h"
#include <QDir>
#include <algorithm>

namespace Mdt{ namespace ExecutableFile{

RPathEntry::RPathEntry(const QString & path) noexcept
  : mPath( QDir::cleanPath( path.trimmed() ) )
{
  assert( !mPath.isEmpty() );
}

bool operator==(const RPathEntry & a, const RPathEntry & b) noexcept
{
  return a.path() == b.path();
}

bool operator==(const RPath & a, const RPath & b) noexcept
{
  if( a.entriesCount() != b.entriesCount() ){
    return false;
  }

  return std::equal( a.cbegin(), a.cend(), b.cbegin() );
}

}} // namespace Mdt{ namespace ExecutableFile{

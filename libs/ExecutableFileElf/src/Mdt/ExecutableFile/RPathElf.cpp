// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RPathElf.h"
#include <QStringList>
#include <QLatin1Char>
#include <QLatin1String>
#include <QStringBuilder>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

RPathEntry RPathElf::rPathEntryFromString(const QString & path)
{
  const QString entryString = path.trimmed();
  assert( !entryString.isEmpty() );

  QString rpathEntryString;

  if( entryString.startsWith( QLatin1String("$ORIGIN") ) ){
    if(entryString.length() > 7){
      if( entryString.at(7) != QLatin1Char('/') ){
        const QString msg = tr("expected a '/' after $ORIGIN, got '%1'")
                            .arg( entryString.at(7) );
        throw RPathFormatError(msg);
      }
      rpathEntryString = entryString.right(entryString.length() - 8);
    }else{
      rpathEntryString = QLatin1String(".");
    }
  }else if( entryString.startsWith( QLatin1String("${ORIGIN}") ) ){
    if(entryString.length() > 9){
      if( entryString.at(9) != QLatin1Char('/') ){
        const QString msg = tr("expected a '/' after ${ORIGIN}, got '%1'")
                            .arg( entryString.at(7) );
        throw RPathFormatError(msg);
      }
      rpathEntryString = entryString.right(entryString.length() - 10);
    }else{
      rpathEntryString = QLatin1String(".");
    }
  }else{
    rpathEntryString = entryString;
  }

  if( rpathEntryString.isEmpty() ){
    rpathEntryString = QLatin1String(".");
  }

  return RPathEntry(rpathEntryString);
}

RPath RPathElf::rPathFromString(const QString & rpathString)
{
  RPath rpath;
  const QStringList rpathStringList = rpathString.split(QLatin1Char(':'), QString::SkipEmptyParts);

  for(const QString & path : rpathStringList){
    rpath.appendEntry( rPathEntryFromString(path) );
  }

  return rpath;
}

QString RPathElf::rPathEntryToString(const RPathEntry & rpathEntry) noexcept
{
  assert( !rpathEntry.path().trimmed().isEmpty() );

  QString path;

  if( rpathEntry.isRelative() ){
    if( rpathEntry.path() == QLatin1String(".") ){
      path = QLatin1String("$ORIGIN");
    }else if( rpathEntry.path().startsWith( QLatin1String("./") ) ){
      path = QLatin1String("$ORIGIN") + rpathEntry.path().right(rpathEntry.path().length() - 1);
    }else{
      path = QLatin1String("$ORIGIN/") + rpathEntry.path();
    }
  }else{
    path = rpathEntry.path();
  }

  return path;
}

QString RPathElf::rPathToString(const RPath & rpath) noexcept
{
  QString rpathString;

  if( !rpath.isEmpty() ){
    rpathString = rPathEntryToString( rpath.entryAt(0) );
    for(size_t i=1; i < rpath.entriesCount(); ++i){
      rpathString += QLatin1Char(':') % rPathEntryToString( rpath.entryAt(i) );
    }
  }

  return rpathString;
}

}} // namespace Mdt{ namespace ExecutableFile{

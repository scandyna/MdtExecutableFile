// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************
 **
 ** MdtDeployUtils - A C++ library to help deploy C++ compiled binaries
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 ****************************************************************************/
#include "TestUtils.h"
#include <QLatin1Char>
#include <QLatin1String>
#include <QDir>
#include <cassert>

Mdt::ExecutableFile::Platform getNonNativePlatform()
{
  using namespace Mdt::ExecutableFile;

  Platform nativePlatform = Platform::nativePlatform();

  if( nativePlatform.executableFileFormat() == ExecutableFileFormat::Elf ){
    return Platform( OperatingSystem::Windows, ExecutableFileFormat::Pe, nativePlatform.compiler(), nativePlatform.processorISA() );
  }
  return Platform( OperatingSystem::Linux, ExecutableFileFormat::Elf, nativePlatform.compiler(), nativePlatform.processorISA() );
}

QStringList qStringListFromUtf8Strings(const std::vector<std::string> & args)
{
  QStringList arguments;

  for(const auto & arg : args){
    arguments.append( QString::fromStdString(arg) );
  }

  return arguments;
}

QString generateStringWithNChars(int n)
{
  assert( n > 0 );

  QString str;

  for(int i=0; i<n; ++i){
    str += QString::number(i%10);
  }
  assert( str.length() == n );

  return str;
}

QStringList getTestPrefixPath(const char * const prefixPath)
{
  return QString::fromLocal8Bit(prefixPath).split( QLatin1Char(',') );
}

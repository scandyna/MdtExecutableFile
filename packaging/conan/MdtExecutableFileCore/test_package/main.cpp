// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include <Mdt/ExecutableFile/ExecutableFileReader.h>
#include <QString>
#include <QDebug>
#include <cassert>

int main(int argc, char **argv)
{
  using Mdt::ExecutableFile::ExecutableFileReader;

  assert( argc == 1 );

  const QString executableFile = QString::fromLocal8Bit(argv[0]);
  int retVal = 1;

  ExecutableFileReader reader;
  reader.openFile(executableFile);
  if( reader.isExecutableOrSharedLibrary() ){
    retVal = 0;
  }else{
    retVal = 1;
  }
  qDebug() << executableFile << " directly depends on: " << reader.getNeededSharedLibraries();
  reader.close();

  return retVal;
}

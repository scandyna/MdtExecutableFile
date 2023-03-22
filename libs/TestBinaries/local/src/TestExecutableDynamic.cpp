// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include <QDebug>
#include <iostream>
#include "TestSharedLibrary.h"

int main(int argc, char **argv)
{
  qDebug() << "Hello dynamic";

  if(argc < 1){
    return 1;
  }

  if( process(argv[0]) != 0 ){
    return 0;
  }

  return 1;
}

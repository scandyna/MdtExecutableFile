// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "TestStaticLibrary.h"
#include <string>
#include <iostream>

void sayHelloStatic()
{
  std::cout << "Hello from static lib" << std::endl;
}

int processStatic(const char *str)
{
  const std::string s(str);

  std::cout << "processStatic() - str: " << s << std::endl;

  if( !s.empty() ){
    return 42;
  }

  return 76;
}

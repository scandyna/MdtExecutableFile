// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "TestFileUtils.h"
#include <QFile>

TEST_CASE("hasExePermissions")
{
  SECTION("Owner Read Write Exe")
  {
    QFile::Permissions permissions{QFile::ReadOwner,QFile::WriteOwner,QFile::ExeOwner};

    REQUIRE( hasExePermissions(permissions) );
  }

  SECTION("Owner Read Write")
  {
    QFile::Permissions permissions{QFile::ReadOwner,QFile::WriteOwner};

    REQUIRE( !hasExePermissions(permissions) );
  }
}

TEST_CASE("setExePermissions")
{
  QFile::Permissions permissions{QFile::ReadOwner,QFile::WriteOwner};

  setExePermissions(permissions);

  REQUIRE( permissions.testFlag(QFile::ReadOwner) );
  REQUIRE( permissions.testFlag(QFile::WriteOwner) );
  REQUIRE( permissions.testFlag(QFile::ExeOwner) );
}

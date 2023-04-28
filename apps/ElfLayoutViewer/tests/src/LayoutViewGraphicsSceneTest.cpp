// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "LayoutViewGraphicsSceneTest.h"
#include "LayoutViewGraphicsScene.h"

using Mdt::ExecutableFile::Elf::SectionHeader;


void LayoutViewGraphicsSceneTest::construct()
{
  LayoutViewGraphicsScene scene;

  QVERIFY( scene.scene() != nullptr );
}

void LayoutViewGraphicsSceneTest::sectionsAreaHeight()
{
  SectionHeader header;

  LayoutViewGraphicsScene scene;
  QCOMPARE( scene.sectionsAreaHeight(), 0.0 );

  header.offset = 10;
  header.size = 20;
  scene.addSection(header);
  QVERIFY( scene.sectionsAreaHeight() > 0.0 );
}

QTEST_MAIN(LayoutViewGraphicsSceneTest)

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
#include <QLatin1String>

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

void LayoutViewGraphicsSceneTest::clearTest()
{
  LayoutViewGraphicsScene scene;
  QCOMPARE( scene.scene()->items().count(), 0 );

  scene.scene()->addSimpleText( QLatin1String("Some text") );
  QCOMPARE( scene.scene()->items().count(), 1 );

  scene.clear();
  QCOMPARE( scene.scene()->items().count(), 0 );
}

QTEST_MAIN(LayoutViewGraphicsSceneTest)

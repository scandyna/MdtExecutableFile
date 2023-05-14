// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LAYOUT_VIEW_GRAPHICS_SCENE_TEST_H
#define LAYOUT_VIEW_GRAPHICS_SCENE_TEST_H

#include <QTest>
#include <QObject>

class LayoutViewGraphicsSceneTest : public QObject
{
  Q_OBJECT

 private slots:

  void construct();
  void sectionsAreaHeight();
  void clearTest();
};

#endif // #ifndef LAYOUT_VIEW_GRAPHICS_SCENE_TEST_H

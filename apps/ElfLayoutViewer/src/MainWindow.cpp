// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "MainWindow.h"

#include "SectionGraphicsItem.h"

#include <QString>
#include <QLatin1String>
#include <QPointF>


MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent)
{
  mUi.setupUi(this);

  /// \todo just a sandbox
  
  SecSegGraphicsItemData interpSection;
  interpSection.offset = 0x238;
  interpSection.size = 0x1c;
  interpSection.name = ".interp";
  
  mScene.addItem( new SectionGraphicsItem(interpSection) );
  
  SecSegGraphicsItemData dynstrSection;
  dynstrSection.offset = 0x2200;
  dynstrSection.size = 0x2a0b;
  dynstrSection.name = ".dynstr";
  
  mScene.addItem( new SectionGraphicsItem(dynstrSection) );

  SecSegGraphicsItemData phdrSeg;
  phdrSeg.offset = 0x40;
  phdrSeg.size = 0x1f8;
  phdrSeg.name = "PHDR";

  auto phdrSegItem = new SectionGraphicsItem(phdrSeg);
  phdrSegItem->moveBy(0.0, 50.0);

  mScene.addItem(phdrSegItem);

  SecSegGraphicsItemData interpSeg;
  interpSeg.offset = 0x238;
  interpSeg.size = 0x1c;
  interpSeg.name = "INTERP";

  auto interpSegItem = new SectionGraphicsItem(interpSeg);
  interpSegItem->moveBy(0.0, 100.0);

  mScene.addItem(interpSegItem);
  
  SecSegGraphicsItemData load1Seg;
  load1Seg.offset = 0;
  load1Seg.size = 0x6eceb;
  load1Seg.name = "LOAD";

  auto load1SegItem = new SectionGraphicsItem(load1Seg);
  load1SegItem->moveBy(0.0, 150.0);

  mScene.addItem(load1SegItem);

//   auto secondItem = new SectionGraphicsItem;
//   QPointF secondItemPos(501.0, 0.0);
//   secondItem->setPos(secondItemPos);
//   mScene.addItem(secondItem);
  
//   mScene.addText( QLatin1String("Hello, world!") );

  mUi.graphicsView->setScene(&mScene);
  mUi.graphicsView->show();
}

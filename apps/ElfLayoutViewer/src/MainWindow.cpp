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

#include <QMatrix>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent)
{
  mUi.setupUi(this);

  /// \todo just a sandbox
  
  SectionGraphicsItemData interpSection;
  interpSection.setOffset(0x238);
  interpSection.setSize(0x1c);
  interpSection.setName(".interp");
  
  mScene.addItem( new SectionGraphicsItem(interpSection) );
  
  SectionGraphicsItemData dynstrSection;
  dynstrSection.setOffset(0x2200);
  dynstrSection.setSize(0x2a0b);
  dynstrSection.setName(".dynstr");

  mScene.addItem( new SectionGraphicsItem(dynstrSection) );

//   SecSegGraphicsItemData phdrSeg;
//   phdrSeg.offset = 0x40;
//   phdrSeg.size = 0x1f8;
//   phdrSeg.name = "PHDR";
// 
//   auto phdrSegItem = new SectionGraphicsItem(phdrSeg);
//   phdrSegItem->moveBy(0.0, 50.0);
// 
//   mScene.addItem(phdrSegItem);
// 
//   SecSegGraphicsItemData interpSeg;
//   interpSeg.offset = 0x238;
//   interpSeg.size = 0x1c;
//   interpSeg.name = "INTERP";
// 
//   auto interpSegItem = new SectionGraphicsItem(interpSeg);
//   interpSegItem->moveBy(0.0, 100.0);
// 
//   mScene.addItem(interpSegItem);
//   
//   SecSegGraphicsItemData load1Seg;
//   load1Seg.offset = 0;
//   load1Seg.size = 0x6eceb;
//   load1Seg.name = "LOAD";
// 
//   auto load1SegItem = new SectionGraphicsItem(load1Seg);
//   load1SegItem->moveBy(0.0, 150.0);
// 
//   mScene.addItem(load1SegItem);

//   auto secondItem = new SectionGraphicsItem;
//   QPointF secondItemPos(501.0, 0.0);
//   secondItem->setPos(secondItemPos);
//   mScene.addItem(secondItem);
  
//   mScene.addText( QLatin1String("Hello, world!") );

  mUi.layoutView->setScene(&mScene);
  mUi.layoutView->show();

  connect(mUi.actionZoom_in, &QAction::triggered, this, &MainWindow::layoutViewZoomIn);
  connect(mUi.actionZoom_out, &QAction::triggered, this, &MainWindow::layoutViewZoomOut);
  connect(mUi.actionZoom_original, &QAction::triggered, this, &MainWindow::layoutViewZoomOriginal);
  connect(mUi.actionZoom_best, &QAction::triggered, this, &MainWindow::layoutViewZoomFitBest);
  
  mUi.actionZoom_best->setEnabled(false);
}

/// \todo should limit the zoom to some bounds

void MainWindow::layoutViewZoomIn()
{
  mUi.layoutView->scale(1.2, 1.0);
  
  qDebug() << "after zoom in matrix: " << mUi.layoutView->matrix();
}

void MainWindow::layoutViewZoomOut()
{
  mUi.layoutView->scale(1.0/1.2, 1.0);
  
  qDebug() << "after zoom out matrix: " << mUi.layoutView->matrix();
}

void MainWindow::layoutViewZoomOriginal()
{
  mUi.layoutView->resetMatrix();
  
  qDebug() << "original matrix: " << mUi.layoutView->matrix();
}

void MainWindow::layoutViewZoomFitBest()
{
  qDebug() << "zoom fit best not implemented..";
}

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
#include "SegmentGraphicsItem.h"

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include <vector>

#include <QString>
#include <QLatin1String>
#include <QPointF>

#include <QTableView>

#include <QItemSelectionModel>

#include <QMatrix>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent)
{
  mUi.setupUi(this);

  /// \todo just a sandbox

  using Mdt::ExecutableFile::Elf::SectionHeader;

  SectionHeader interpSection;
  interpSection.offset = 0x238;
  interpSection.size = 0x1c;
  interpSection.name = ".interp";

//   SectionGraphicsItemData interpSection;
//   interpSection.setOffset(0x238);
//   interpSection.setSize(0x1c);
//   interpSection.setName(".interp");

//   mFakeCurrentSectionItem = new SectionGraphicsItem(interpSection);
  auto item = new SectionGraphicsItem( SectionGraphicsItemData::fromSectionHeader(interpSection) );

  mScene.addItem( item );

  auto id = mSectionHeaderTableGraphicsItemMap.registerItem(item);

  mSectionHeaderTableModel.addSection(interpSection, id);


  SectionHeader dynstrSection;
  dynstrSection.offset = 0x2200;
  dynstrSection.size = 0x2a0b;
  dynstrSection.name = ".dynstr";

//   SectionGraphicsItemData dynstrSection;
//   dynstrSection.setOffset(0x2200);
//   dynstrSection.setSize(0x2a0b);
//   dynstrSection.setName(".dynstr");

  item = new SectionGraphicsItem( SectionGraphicsItemData::fromSectionHeader(dynstrSection) );
  mScene.addItem(item);

  id = mSectionHeaderTableGraphicsItemMap.registerItem(item);

  mSectionHeaderTableModel.addSection(dynstrSection, id);


  const qreal firstSegmentY = 100.0;
  qreal segmentY = firstSegmentY;

  SegmentGraphicsItemData phdrSeg;
  phdrSeg.setOffset(0x40);
  phdrSeg.setSize(0x1f8);
  phdrSeg.setName("PHDR");

  auto phdrSegItem = new SegmentGraphicsItem(phdrSeg);
  phdrSegItem->moveBy(0.0, segmentY);
  mScene.addItem(phdrSegItem);


  SegmentGraphicsItemData interpSeg;
  interpSeg.setOffset(0x238);
  interpSeg.setSize(0x1c);
  interpSeg.setName("INTERP");

  auto interpSegItem = new SegmentGraphicsItem(interpSeg);
  interpSegItem->moveBy(0.0, segmentY);
  mScene.addItem(interpSegItem);

  SegmentGraphicsItemData load1Seg;
  load1Seg.setOffset(0);
  load1Seg.setSize(0x6eceb);
  load1Seg.setName("LOAD");

  auto load1SegItem = new SegmentGraphicsItem(load1Seg);
  segmentY += load1SegItem->boundingRect().height();
  load1SegItem->moveBy(0.0, segmentY);
  mScene.addItem(load1SegItem);


  mUi.layoutView->setScene(&mScene);
  mUi.layoutView->centerOn(0.0, 0.0);
  mUi.layoutView->show();

  connect(mUi.actionZoom_in, &QAction::triggered, this, &MainWindow::layoutViewZoomIn);
  connect(mUi.actionZoom_out, &QAction::triggered, this, &MainWindow::layoutViewZoomOut);
  connect(mUi.actionZoom_original, &QAction::triggered, this, &MainWindow::layoutViewZoomOriginal);
  connect(mUi.actionZoom_best, &QAction::triggered, this, &MainWindow::layoutViewZoomFitBest);

  mUi.actionZoom_best->setEnabled(false);


  mUi.sectionHeaderTableView->setModel(&mSectionHeaderTableModel);
  mUi.sectionHeaderTableView->resizeColumnsToContents();
  mUi.sectionHeaderTableView->resizeRowsToContents();

  /// \todo use something "selected", maybe from selection model
//   connect(mUi.sectionHeaderTableView, &QTableView::clicked, this, &MainWindow::selectItemInLayoutView);
//   connect(mUi.sectionHeaderTableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::selectItemInLayoutView);
  connect(mUi.sectionHeaderTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::selectItemInLayoutView);

  connect(mUi.actionTrack_selected_item, &QAction::toggled, this, &MainWindow::setTrackSelectedItem);
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

void MainWindow::setTrackSelectedItem(bool enable)
{
  mTrackSelectedItem = enable;
  qDebug() << "Track selected item: " << mTrackSelectedItem;
}

void MainWindow::selectItemInLayoutView(const QModelIndex & current, const QModelIndex & previous)
{
  /// \todo Important: map to the model index !
  qDebug() << "current: " << current << " , previous" << previous;

  auto item = mSectionHeaderTableGraphicsItemMap.itemForId( mSectionHeaderTableModel.data(current, Qt::UserRole).toInt() );

  item->setHighlighted(true);
  if(mTrackSelectedItem){
    mUi.layoutView->centerOn(item);
  }

  if( previous.isValid() ){
    item = mSectionHeaderTableGraphicsItemMap.itemForId( mSectionHeaderTableModel.data(previous, Qt::UserRole).toInt() );
    item->setHighlighted(false);
  }


//   if( index.row() == 2 ){
//     mFakeCurrentSectionItem->setHighlighted(true);
//     if(mTrackSelectedItem){
//       mUi.layoutView->centerOn(mFakeCurrentSectionItem);
//     }
//   }else{
//     mFakeCurrentSectionItem->setHighlighted(false);
//   }
}

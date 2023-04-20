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
  using Mdt::ExecutableFile::Elf::ProgramHeader;
  using Mdt::ExecutableFile::Elf::SegmentType;

  SectionHeader interpSection;
  interpSection.offset = 0x238;
  interpSection.size = 0x1c;
  interpSection.name = ".interp";


//   mFakeCurrentSectionItem = new SectionGraphicsItem(interpSection);
  auto item = new SectionGraphicsItem( SectionGraphicsItemData::fromSectionHeader(interpSection) );

  mScene.addItem( item );

  auto id = mSectionHeaderTableGraphicsItemMap.registerItem(item);

  mSectionHeaderTableModel.addSection(interpSection, id);


  SectionHeader dynstrSection;
  dynstrSection.offset = 0x2200;
  dynstrSection.size = 0x2a0b;
  dynstrSection.name = ".dynstr";

  item = new SectionGraphicsItem( SectionGraphicsItemData::fromSectionHeader(dynstrSection) );
  mScene.addItem(item);

  id = mSectionHeaderTableGraphicsItemMap.registerItem(item);

  mSectionHeaderTableModel.addSection(dynstrSection, id);


  const qreal firstSegmentY = 100.0;
  qreal segmentY = firstSegmentY;

  ProgramHeader phdrHeader;
  phdrHeader.setSegmentType(SegmentType::ProgramHeaderTable);
  phdrHeader.offset = 0x40;
  phdrHeader.filesz = 0x1f8;

  auto phdrSegItem = new SegmentGraphicsItem( SegmentGraphicsItemData::fromProgramHeader(phdrHeader) );
  phdrSegItem->moveBy(0.0, segmentY);
  mScene.addItem(phdrSegItem);

  id = mProgramHeaderTableGraphicsItemMap.registerItem(phdrSegItem);
  mProgramHeaderTableModel.addSegment(phdrHeader, id);


  ProgramHeader interpHeader;
  interpHeader.setSegmentType(SegmentType::Interpreter);
  interpHeader.offset = 0x238;
  interpHeader.filesz = 0x1c;


  auto interpSegItem = new SegmentGraphicsItem( SegmentGraphicsItemData::fromProgramHeader(interpHeader) );
  interpSegItem->moveBy(0.0, segmentY);
  mScene.addItem(interpSegItem);

  id = mProgramHeaderTableGraphicsItemMap.registerItem(interpSegItem);
  mProgramHeaderTableModel.addSegment(interpHeader, id);


  ProgramHeader load1Header;
  load1Header.setSegmentType(SegmentType::Load);
  load1Header.offset = 0;
  load1Header.filesz = 0x6eceb;

  auto load1SegItem = new SegmentGraphicsItem( SegmentGraphicsItemData::fromProgramHeader(load1Header) );
  segmentY += load1SegItem->boundingRect().height();
  load1SegItem->moveBy(0.0, segmentY);
  mScene.addItem(load1SegItem);

  id = mProgramHeaderTableGraphicsItemMap.registerItem(load1SegItem);
  mProgramHeaderTableModel.addSegment(load1Header, id);


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

  mUi.programHeaderTableView->setModel(&mProgramHeaderTableModel);
  mUi.programHeaderTableView->resizeColumnsToContents();
  mUi.programHeaderTableView->resizeRowsToContents();


  /// \todo should also react when entry is selected or clicked in a table view
//   connect(mUi.sectionHeaderTableView, &QTableView::clicked, this, &MainWindow::selectItemInLayoutView);
//   connect(mUi.sectionHeaderTableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::selectItemInLayoutView);
  connect(mUi.sectionHeaderTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::selectSectionItemInLayoutView);
  connect(mUi.programHeaderTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::selectSegmentItemInLayoutView);

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

void MainWindow::selectSectionItemInLayoutView(const QModelIndex & current, const QModelIndex & previous)
{
  /// \todo Important: map to the model index !
  qDebug() << "current: " << current << " , previous" << previous;

  /// \todo maybe check returned variant
  auto id = HeaderTableGraphicsItemMapId::fromQVariant( mSectionHeaderTableModel.data(current, Qt::UserRole) );
  auto item = mSectionHeaderTableGraphicsItemMap.itemForId(id);

  item->setHighlighted(true);
  if(mTrackSelectedItem){
    mUi.layoutView->centerOn(item);
  }

  if( previous.isValid() ){
    /// \todo maybe check returned variant
    id = HeaderTableGraphicsItemMapId::fromQVariant( mSectionHeaderTableModel.data(previous, Qt::UserRole) );
    item = mSectionHeaderTableGraphicsItemMap.itemForId(id);
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

void MainWindow::selectSegmentItemInLayoutView(const QModelIndex & current, const QModelIndex & previous)
{
  /// \todo maybe check returned variant
  auto id = HeaderTableGraphicsItemMapId::fromQVariant( mProgramHeaderTableModel.data(current, Qt::UserRole) );
  auto item = mProgramHeaderTableGraphicsItemMap.itemForId(id);

  item->setHighlighted(true);
  if(mTrackSelectedItem){
    mUi.layoutView->centerOn(item);
  }

  if( previous.isValid() ){
    /// \todo maybe check returned variant
    id = HeaderTableGraphicsItemMapId::fromQVariant( mProgramHeaderTableModel.data(previous, Qt::UserRole) );
    item = mProgramHeaderTableGraphicsItemMap.itemForId(id);
    item->setHighlighted(false);
  }
}

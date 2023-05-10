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
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeaderTable.h"
#include "Mdt/ExecutableFile/ElfFileIoEngine.h"
#include <vector>

#include <QString>
#include <QLatin1String>
#include <QPointF>

#include <QTableView>

#include <QItemSelectionModel>

#include <QMatrix>
#include <QDebug>

#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent)
{
  mUi.setupUi(this);

  /// \todo just a sandbox

  using Mdt::ExecutableFile::Elf::SectionHeader;
  using Mdt::ExecutableFile::Elf::ProgramHeader;
  using Mdt::ExecutableFile::Elf::SegmentType;

  readFile();

  mUi.layoutView->setScene( mScene.scene() );
  mUi.layoutView->centerOn(0.0, 0.0);
  mUi.layoutView->show();

  connect(mUi.actionZoom_in, &QAction::triggered, this, &MainWindow::layoutViewZoomIn);
  connect(mUi.actionZoom_out, &QAction::triggered, this, &MainWindow::layoutViewZoomOut);
  connect(mUi.actionZoom_original, &QAction::triggered, this, &MainWindow::layoutViewZoomOriginal);
  connect(mUi.actionZoom_best, &QAction::triggered, this, &MainWindow::layoutViewZoomFitBest);

  mUi.actionZoom_best->setEnabled(false);


  mSectionHeaderTableSortFilterModel.setSourceModel(&mSectionHeaderTableModel);
  mSectionHeaderTableSortFilterModel.setSortRole( mSectionHeaderTableModel.sortRole() );
  mUi.sectionHeaderTableView->setModel(&mSectionHeaderTableSortFilterModel);
  mUi.sectionHeaderTableView->resizeColumnsToContents();
  mUi.sectionHeaderTableView->resizeRowsToContents();

  mProgramHeaderTableSortFilterModel.setSourceModel(&mProgramHeaderTableModel);
  mProgramHeaderTableSortFilterModel.setSortRole( mProgramHeaderTableModel.sortRole() );
  mUi.programHeaderTableView->setModel(&mProgramHeaderTableSortFilterModel);
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

void MainWindow::selectSectionItemInLayoutView(const QModelIndex & viewCurrent, const QModelIndex & viewPrevious)
{
  /*
   * Given indexes comes from the view
   * (or its selection model).
   * We have to map them to the model.
   */
  const QModelIndex current = mSectionHeaderTableSortFilterModel.mapToSource(viewCurrent);
  const QModelIndex previous = mSectionHeaderTableSortFilterModel.mapToSource(viewPrevious);

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
}

void MainWindow::selectSegmentItemInLayoutView(const QModelIndex & viewCurrent, const QModelIndex & viewPrevious)
{
  /*
   * Given indexes comes from the view
   * (or its selection model).
   * We have to map them to the model.
   */
  const QModelIndex current = mProgramHeaderTableSortFilterModel.mapToSource(viewCurrent);
  const QModelIndex previous = mProgramHeaderTableSortFilterModel.mapToSource(viewPrevious);

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

/// \todo just a sandbox
void MainWindow::readFile()
{
  using Mdt::ExecutableFile::ElfFileIoEngine;
  using Mdt::ExecutableFile::ExecutableFileOpenMode;
  using Mdt::ExecutableFile::Elf::SectionHeaderTable;
  using Mdt::ExecutableFile::Elf::ProgramHeaderTable;

  const QString filePath = QCoreApplication::applicationFilePath();

  ElfFileIoEngine reader;
  reader.openFile(filePath, ExecutableFileOpenMode::ReadOnly);
  const SectionHeaderTable sectionHeaderTable = reader.getSectionHeaderTable();
  const ProgramHeaderTable programHeaderTable = reader.getProgramHeaderTable();
  reader.close();

  for(const auto & header : sectionHeaderTable){
    addSection(header);
  }

  for(const auto & header : programHeaderTable){
    addSegment(header);
  }
}

void MainWindow::addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header) noexcept
{
  auto item = mScene.addSection(header);

  const auto id = mSectionHeaderTableGraphicsItemMap.registerItem(item);

  mSectionHeaderTableModel.addSection(header, id);
}

void MainWindow::addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept
{
  auto item = mScene.addSegment(header);

  auto id = mProgramHeaderTableGraphicsItemMap.registerItem(item);

  mProgramHeaderTableModel.addSegment(header, id);
}

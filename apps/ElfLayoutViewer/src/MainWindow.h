// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "HeaderTableGraphicsItemMap.h"
#include "LayoutViewGraphicsScene.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include "ui_MainWindow.h"
#include <QMainWindow>

#include <QString>

#include <QGraphicsScene>

#include "SectionHeaderTableModel.h"
#include "ProgramHeaderTableModel.h"
#include <QSortFilterProxyModel>

#include <QModelIndex>

#include "SectionGraphicsItem.h"
#include "SegmentGraphicsItem.h"

#include <vector>



/*! \internal
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:

  explicit MainWindow(QWidget *parent = nullptr);

 private slots:

  void openFile();

  void layoutViewZoomIn();
  void layoutViewZoomOut();
  void layoutViewZoomOriginal();
  void layoutViewZoomFitBest();

  void setTrackSelectedItem(bool enable);
  void selectSectionItemInLayoutView(const QModelIndex & viewCurrent, const QModelIndex & viewPrevious);
  void selectSegmentItemInLayoutView(const QModelIndex & viewCurrent, const QModelIndex & viewPrevious);


 private:

  void clear();
  void readFile(const QString &filePath);
  void addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header) noexcept;
  void addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept;

  LayoutViewGraphicsScene mScene;
  QSortFilterProxyModel mSectionHeaderTableSortFilterModel;
  SectionHeaderTableModel mSectionHeaderTableModel;
  QSortFilterProxyModel mProgramHeaderTableSortFilterModel;
  ProgramHeaderTableModel mProgramHeaderTableModel;
  Ui::MainWindow mUi;

  bool mTrackSelectedItem = false;

  HeaderTableGraphicsItemMap mSectionHeaderTableGraphicsItemMap;
  HeaderTableGraphicsItemMap mProgramHeaderTableGraphicsItemMap;
};

#endif // #ifndef MAIN_WINDOW_H

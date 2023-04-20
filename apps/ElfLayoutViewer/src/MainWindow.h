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
#include "ui_MainWindow.h"
#include <QMainWindow>

#include <QGraphicsScene>

#include "SectionHeaderTableModel.h"
#include "ProgramHeaderTableModel.h"

#include <QModelIndex>

#include "SectionGraphicsItem.h"

#include <vector>


/*! \brief
 */
class SectionHeaderTableGraphicsItemMap
{
 private:

  HeaderTableGraphicsItemMap mMap;
};

/*! \brief
 */
class ProgramHeaderTableGraphicsItemMap
{
 private:

  HeaderTableGraphicsItemMap mMap;
};

/*! \internal
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:

  explicit MainWindow(QWidget *parent = nullptr);

 private slots:

  void layoutViewZoomIn();
  void layoutViewZoomOut();
  void layoutViewZoomOriginal();
  void layoutViewZoomFitBest();

  void setTrackSelectedItem(bool enable);
  void selectSectionItemInLayoutView(const QModelIndex & current, const QModelIndex & previous);
  void selectSegmentItemInLayoutView(const QModelIndex & current, const QModelIndex & previous);


 private:

  QGraphicsScene mScene;
  SectionHeaderTableModel mSectionHeaderTableModel;
  ProgramHeaderTableModel mProgramHeaderTableModel;
  Ui::MainWindow mUi;

//   SectionGraphicsItem *mFakeCurrentSectionItem;
  bool mTrackSelectedItem = false;

  HeaderTableGraphicsItemMap mSectionHeaderTableGraphicsItemMap;
  HeaderTableGraphicsItemMap mProgramHeaderTableGraphicsItemMap;
};

#endif // #ifndef MAIN_WINDOW_H

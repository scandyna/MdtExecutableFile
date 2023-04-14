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

#include "ui_MainWindow.h"
#include <QMainWindow>

#include <QGraphicsScene>

#include "SectionHeaderTableModel.h"

#include <QModelIndex>

#include "SectionGraphicsItem.h"

#include <vector>

/*! \brief
 *
 * When the user selects a section in the section header table
 * or a segment in the program header table,
 * we want to highlight it in the layout view.
 *
 * To achieve this, we have to find the graphics item in the layout view
 * that corresponds to the selected item in the header table view.
 *
 * Each time a new item is added to the map, a ID is returned.
 * This ID is simply the index in the map
 * (internally, the map is implemented as a vector).
 *
 * The returned ID has to be stored in the corresponding table model as user data.
 *
 *
 * \section Rationale Rationale
 *
 * Index by name ?
 * -> does not work for segments (many ones called LOAD)
 * (maybe slow lookup)
 *
 * Index by model index ?
 * -> what about proxy models (sort, maybe others) ?
 *
 * Index by offset ?
 * -> does not work for segments (many ones could start from the same offset)
 * -> we want to ba able to display corrupted files (having f.ex. 2 section starting from same offset
 *
 * Index by some ID ?
 */
class HeaderTableGraphicsItemMap
{
 public:

  /*! \brief
   */
  int registerItem(LayoutGraphicsItem *item) noexcept
  {
    mList.push_back(item);

    return mList.size() - 1;
  }

  LayoutGraphicsItem *itemForId(int id) const noexcept
  {
    return mList[id];
  }

 private:

  std::vector<LayoutGraphicsItem*> mList;
};

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
  void selectItemInLayoutView(const QModelIndex & current, const QModelIndex & previous);


 private:

  QGraphicsScene mScene;
  SectionHeaderTableModel mSectionHeaderTableModel;
  Ui::MainWindow mUi;

//   SectionGraphicsItem *mFakeCurrentSectionItem;
  bool mTrackSelectedItem = false;

  HeaderTableGraphicsItemMap mSectionHeaderTableGraphicsItemMap;
};

#endif // #ifndef MAIN_WINDOW_H

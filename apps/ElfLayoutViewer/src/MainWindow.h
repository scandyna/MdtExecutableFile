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

 private:

  QGraphicsScene mScene;
  Ui::MainWindow mUi;
};

#endif // #ifndef MAIN_WINDOW_H

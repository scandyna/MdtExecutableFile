// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ABSTRACT_TABLE_MODEL_H
#define ABSTRACT_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QModelIndexList>
#include <QVariant>

/*! \brief Base to implement a Qt table model
 */
class AbstractTableModel : public QAbstractTableModel
{
  Q_OBJECT

 public:

  /*! \brief Constructor
   */
  explicit AbstractTableModel(QObject *parent = nullptr);

  /*! \brief Returns data
   */
  QVariant data(const QModelIndex & index, int role) const override;

  /*! \brief Get the header data
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

 protected:

  bool indexIsValidAndInRange(const QModelIndex & index) const noexcept;

  virtual
  QVariant displayRoleData(const QModelIndex & index) const noexcept = 0;

  virtual
  QVariant userRoleData(const QModelIndex & index) const noexcept = 0;

  virtual
  QVariant horizontalDisplayRoleHeaderData(int columnNumber) const noexcept = 0;
};

#endif // #ifndef ABSTRACT_TABLE_MODEL_H

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

  /*! \brief Get the value for the sort role
   */
  int sortRole() const noexcept
  {
    return Qt::UserRole + 1;
  }

  /*! \brief Returns data
   */
  QVariant data(const QModelIndex & index, int role) const override;

  /*! \brief Get the header data
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  /*! \brief Prepare this model to add new rows
   *
   * Design choice:
   * to add rows to a Qt item model, beginInsertRows() and endInsertRows()
   * should be used. This way, the view can represent the actual data
   * each time a row have been inserted.
   * In our case, editing an ELF file is not supported,
   * meaning that using above methods has no sense.
   * This is why using a complete model reset after populating the data have been choosed.
   */
  void prepareToAddRows() noexcept;

  /*! \brief Check if this model is ready to add rows
   *
   * \sa prepareToAddRows()
   */
  bool isReadyToAddRows() const noexcept
  {
    return mIsReadeyToAddRows;
  }

  /*! \brief Commit the added rows
   *
   * \pre This model must have been prepared to add rows (i.e. must be ready to add rows)
   * \sa isReadyToAddRows()
   * \sa prepareToAddRows()
   */
  void commitAddedRows() noexcept;

 protected:

  bool indexIsValidAndInRange(const QModelIndex & index) const noexcept;

  virtual
  QVariant displayRoleData(const QModelIndex & index) const noexcept = 0;

  virtual
  QVariant userRoleData(const QModelIndex & index) const noexcept = 0;

  virtual
  QVariant sortRoleData(const QModelIndex & index) const noexcept = 0;

  virtual
  QVariant horizontalDisplayRoleHeaderData(int columnNumber) const noexcept = 0;

 private:

  bool mIsReadeyToAddRows = false;
};

#endif // #ifndef ABSTRACT_TABLE_MODEL_H

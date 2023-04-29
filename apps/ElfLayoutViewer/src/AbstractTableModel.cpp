// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "AbstractTableModel.h"

AbstractTableModel::AbstractTableModel(QObject *parent)
 : QAbstractTableModel(parent)
{
}

QVariant AbstractTableModel::data(const QModelIndex & index, int role) const
{
  if( !indexIsValidAndInRange(index) ){
    return QVariant();
  }

  if(role == Qt::DisplayRole){
    return displayRoleData(index);
  }

  if(role == Qt::UserRole){
    return userRoleData(index);
  }

  if( role == sortRole() ){
    return sortRoleData(index);
  }

  return QVariant();
}

QVariant AbstractTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(orientation != Qt::Horizontal){
    return QAbstractTableModel::headerData(section, orientation, role);
  }

  if(role == Qt::DisplayRole){
    return horizontalDisplayRoleHeaderData(section);
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

bool AbstractTableModel::indexIsValidAndInRange(const QModelIndex & index) const noexcept
{
  if( !index.isValid() ){
    return false;
  }

  const int row = index.row();
  if(row < 0){
    return false;
  }
  if( row >= rowCount() ){
    return false;
  }

  const int column = index.column();
  if(column < 0){
    return false;
  }
  if( column >= columnCount() ){
    return false;
  }

  return true;
}

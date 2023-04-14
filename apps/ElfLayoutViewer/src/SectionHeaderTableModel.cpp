// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SectionHeaderTableModel.h"

// #include <QLatin1String>

/// \todo for tests, see also https://doc.qt.io/qt-5/qabstractitemmodeltester.html

SectionHeaderTableModel::SectionHeaderTableModel(QObject *parent)
 : QAbstractTableModel(parent)
{
}

void SectionHeaderTableModel::addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header, int id) noexcept
{
  SectionHeaderTableModelData data;
  data.header = header;
  data.id = id;

  mTable.push_back(data);
}

// void SectionHeaderTableModel::setTable(const SectionHeaderTable & table) noexcept
// {
//   mTable = table;
// }

int SectionHeaderTableModel::rowCount(const QModelIndex & parent) const
{
  return mTable.size();
}

int SectionHeaderTableModel::columnCount(const QModelIndex & parent) const
{
  return 3;
}

/// \todo review file:///usr/share/qt5/doc/qtwidgets/model-view-programming.html#model-classes

/// \todo handle index

/// \todo also add header index ? is it artificial ? read order ? Should be added when enable sorting

QVariant SectionHeaderTableModel::data(const QModelIndex & index, int role) const
{
  /**
  if( !indexIsValidAndInRange(index) ){
    return QVariant();
  }
  */
  
  const size_t row = index.row();

  if(role == Qt::DisplayRole){
    switch( index.column() ){
      case 0:
        return QString::fromStdString( mTable[row].header.name );
      case 1:
        return (qulonglong)mTable[row].header.offset;  /// \todo format
      case 2:
        return (qulonglong)mTable[row].header.size;    /// \todo format
    }
//     return tr("Fake 0x123 (25)");
  }
  
  if(role == Qt::UserRole){
    return mTable[row].id;
  }

  return QVariant();
}

QVariant SectionHeaderTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(orientation != Qt::Horizontal){
    return QAbstractTableModel::headerData(section, orientation, role);
  }

  if(role == Qt::DisplayRole){
    switch(section){
      case 0:
        return tr("name");
      case 1:
        return tr("offset");
      case 2:
        return tr("size");
      default:
        return QVariant();
    }
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

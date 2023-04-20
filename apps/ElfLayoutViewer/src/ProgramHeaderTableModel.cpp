// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ProgramHeaderTableModel.h"
#include "Numeric.h"
#include "SegmentTypeString.h"
#include <QLatin1String>

ProgramHeaderTableModel::ProgramHeaderTableModel(QObject *parent)
 : AbstractTableModel(parent)
{
}

void ProgramHeaderTableModel::addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header, HeaderTableGraphicsItemMapId id) noexcept
{
  mTable.emplace_back(header, id);
}

int ProgramHeaderTableModel::rowCount(const QModelIndex & parent) const
{
  if( parent.isValid() ){
    return 0;
  }

  const std::size_t row = mTable.size();

  if( int_canHoldValueOf_size_t(row) ){
    return static_cast<int>(row);
  }

  return 0;
}

int ProgramHeaderTableModel::columnCount(const QModelIndex & parent) const
{
  if( parent.isValid() ){
    return 0;
  }

  return 3;
}

QString ProgramHeaderTableModel::typeToString(Mdt::ExecutableFile::Elf::SegmentType type) const noexcept
{
  return segmentTypeShortName(type);
}

QString ProgramHeaderTableModel::offsetToString(uint64_t offset) const noexcept
{
  const qulonglong ulongOffset = qulonglong_from_uint64(offset);

  return QString( QLatin1String("0x%1 (%2)") )
         .arg(ulongOffset, 0, 16)
         .arg(ulongOffset);
}

QString ProgramHeaderTableModel::sizeToString(uint64_t size) const noexcept
{
  const qulonglong ulongSize = qulonglong_from_uint64(size);

  return QString( QLatin1String("%1 (0x%2)") )
         .arg(ulongSize)
         .arg(ulongSize, 0, 16);
}

QVariant ProgramHeaderTableModel::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  assert( index.row() >= 0 );
  const size_t row = static_cast<size_t>( index.row() );

  const auto column = static_cast<Column>( index.column() );
  switch(column){
    case Column::Type:
      return typeToString( mTable[row].mHeader.segmentType() );
    case Column::Offset:
      return offsetToString(mTable[row].mHeader.offset);
    case Column::Size:
      return sizeToString(mTable[row].mHeader.filesz);
  }

  return QVariant();
}

QVariant ProgramHeaderTableModel::userRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  assert( index.row() >= 0 );
  const size_t row = static_cast<size_t>( index.row() );

  return mTable[row].mId.toQVariant();
}

QVariant ProgramHeaderTableModel::horizontalDisplayRoleHeaderData(int columnNumber) const noexcept
{
  const auto column = static_cast<Column>(columnNumber);

  switch(column){
    case Column::Type:
      return tr("type");
    case Column::Offset:
      return tr("offset");
    case Column::Size:
      return tr("size");
  }

  return QVariant();
}

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SECTION_HEADER_TABLE_MODEL_H
#define SECTION_HEADER_TABLE_MODEL_H

#include "AbstractTableModel.h"
#include "HeaderTableGraphicsItemMapId.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <vector>
#include <cstdint>

/*! \internal
 */
struct SectionHeaderTableModelData
{
  using SectionHeader = Mdt::ExecutableFile::Elf::SectionHeader;

  SectionHeaderTableModelData(const SectionHeader & header, HeaderTableGraphicsItemMapId id) noexcept
   : mHeader(header),
     mId(id)
  {
  }

  SectionHeader mHeader;
  HeaderTableGraphicsItemMapId mId;
};

/*! \brief Qt item model to represent a section header table
 */
class SectionHeaderTableModel : public AbstractTableModel
{
  Q_OBJECT

 public:

  /*! \brief Column enum
   */
  enum class Column
  {
    Name = 0,
    Offset = 1,
    Size = 2
  };

  /*! \brief Constructor
   */
  explicit SectionHeaderTableModel(QObject *parent = nullptr);

  /*! \brief Add a section from given header to this table
   */
  void addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header, HeaderTableGraphicsItemMapId id) noexcept;

  /*! \brief Returns the row count
   */
  int rowCount( const QModelIndex & parent = QModelIndex() ) const override;

  /*! \brief Returns the column count
   */
  int columnCount( const QModelIndex & parent = QModelIndex() ) const override;

 private:

  int rowCountFromTableSize() const noexcept;
  QString offsetToString(uint64_t offset) const noexcept;
  QString sizeToString(uint64_t size) const noexcept;
  QVariant displayRoleData(const QModelIndex & index) const noexcept override;
  QVariant userRoleData(const QModelIndex & index) const noexcept override;
  QVariant horizontalDisplayRoleHeaderData(int columnNumber) const noexcept override;

  std::vector<SectionHeaderTableModelData> mTable;
};

#endif // #ifndef SECTION_HEADER_TABLE_MODEL_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef PROGRAM_HEADER_TABLE_MODEL_H
#define PROGRAM_HEADER_TABLE_MODEL_H

#include "AbstractTableModel.h"
#include "HeaderTableGraphicsItemMapId.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <QVariant>
#include <QString>
#include <vector>

/*! \internal
 */
struct ProgramHeaderTableModelData
{
  using ProgramHeader = Mdt::ExecutableFile::Elf::ProgramHeader;

  ProgramHeaderTableModelData(const ProgramHeader & header, HeaderTableGraphicsItemMapId id) noexcept
   : mHeader(header),
     mId(id)
  {
  }

  ProgramHeader mHeader;
  HeaderTableGraphicsItemMapId mId;
};

/*! \brief Qt item model to represent a program header table
 */
class ProgramHeaderTableModel : public AbstractTableModel
{
  Q_OBJECT

 public:

  /*! \brief Column enum
   */
  enum class Column
  {
    Type = 0,
    Offset = 1,
    Size = 2
  };

  /*! \brief Constructor
   */
  explicit ProgramHeaderTableModel(QObject *parent = nullptr);

  /*! \brief Add a segment from given header to this table
   *
   * \pre This model must be ready to add rows
   * \sa prepareToAddRows()
   * \sa isReadyToAddRows()
   * \sa commitAddedRows()
   */
  void addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header, HeaderTableGraphicsItemMapId id) noexcept;

  /*! \brief Clear this model
   */
  void clear() noexcept;

  /*! \brief Returns the row count
   */
  int rowCount( const QModelIndex & parent = QModelIndex() ) const override;

  /*! \brief Returns the column count
   */
  int columnCount( const QModelIndex & parent = QModelIndex() ) const override;

 private:

  QString typeToString(Mdt::ExecutableFile::Elf::SegmentType type) const noexcept;
  QString offsetToString(uint64_t offset) const noexcept;
  QString sizeToString(uint64_t size) const noexcept;
  QVariant displayRoleData(const QModelIndex & index) const noexcept override;
  QVariant userRoleData(const QModelIndex & index) const noexcept override;
  QVariant sortRoleData(const QModelIndex & index) const noexcept override;
  QVariant horizontalDisplayRoleHeaderData(int columnNumber) const noexcept override;

  std::vector<ProgramHeaderTableModelData> mTable;
};

#endif // #ifndef PROGRAM_HEADER_TABLE_MODEL_H

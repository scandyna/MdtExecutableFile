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

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <vector>

/*! \internal
 */
struct SectionHeaderTableModelData
{
  Mdt::ExecutableFile::Elf::SectionHeader header;
  int id;
};

/*! \brief Qt item model to represent a section header table
 */
class SectionHeaderTableModel : public QAbstractTableModel
{
  Q_OBJECT

 public:

  /*! \brief Section header table
   *
   * \note Current version on the library does not define a proper type for a section header table
   */
//   using SectionHeaderTable = std::vector<Mdt::ExecutableFile::Elf::SectionHeader>;

  /*! \brief Constructor
   */
  explicit SectionHeaderTableModel(QObject *parent = nullptr);

  /*! \brief
   */
  void addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header, int id) noexcept;

  /*! \brief
   */
//   void setTable(const SectionHeaderTable & table) noexcept;

  /*! \brief Returns the row count
   */
  int rowCount(const QModelIndex & parent) const override;

  /*! \brief Returns the column count
   */
  int columnCount(const QModelIndex & parent) const override;

  /*! \brief Returns data
   */
  QVariant data(const QModelIndex & index, int role) const override;

  /*! \brief
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

 private:

  bool indexIsInRange(const QModelIndex & index) const noexcept;
  bool indexIsValidAndInRange(const QModelIndex & index) const noexcept;

  std::vector<SectionHeaderTableModelData> mTable;
};

#endif // #ifndef SECTION_HEADER_TABLE_MODEL_H

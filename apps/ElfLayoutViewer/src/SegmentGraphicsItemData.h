// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SEGMENT_GRAPHICS_ITEM_DATA_H
#define SEGMENT_GRAPHICS_ITEM_DATA_H

#include "Numeric.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <QtGlobal>
#include <QString>
#include <cstdint>


/*! \brief DTO for SegmentGraphicsItem
 */
class SegmentGraphicsItemData
{
 public:

  /*! \brief Set the offset from the program header
   */
  void setOffset(uint64_t offset) noexcept
  {
    mOffset = qulonglong_from_uint64(offset);
  }

  /*! \brief Get the offset as a Qt friendly integer
   */
  qulonglong offset() const noexcept
  {
    return mOffset;
  }

  /*! \brief Get the offset as a Qt friendly floating point
   */
  qreal offsetF() const noexcept
  {
    return qreal_from_qulonglong(mOffset);
  }

  /*! \brief Set the size from the section header
   */
  void setSize(uint64_t size) noexcept
  {
    mSize = qulonglong_from_uint64(size);
  }

  /*! \brief Get the size as a Qt friendly integer
   */
  qulonglong size() const noexcept
  {
    return mSize;
  }

  /*! \brief Get the size as a Qt friendly floating point
   */
  qreal sizeF() const noexcept
  {
    return qreal_from_qulonglong(mSize);
  }

  /*! \brief Get the last address
   *
   * \note for a empty section, the last address is equal to the offset.
   *  This is different from the STL end concept.
   */
  qulonglong lastAddress() const noexcept
  {
    if(mSize == 0){
      return mOffset;
    }
    return mOffset + mSize - 1;
  }

  /*! \brief Set the name
   */
  void setName(const std::string & name) noexcept
  {
    mName = QString::fromStdString(name);
  }

  /*! \brief Get the name
   */
  const QString & name() const noexcept
  {
    return mName;
  }

  static
  SegmentGraphicsItemData fromProgramHeader(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept;

 private:

  qulonglong mOffset = 0;
  qulonglong mSize = 0;
  QString mName;
};

#endif // #ifndef SEGMENT_GRAPHICS_ITEM_DATA_H

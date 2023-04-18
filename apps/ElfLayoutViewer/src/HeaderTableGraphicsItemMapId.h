// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef HEADER_TABLE_GRAPHICS_ITEM_MAP_ID_H
#define HEADER_TABLE_GRAPHICS_ITEM_MAP_ID_H

#include <cstddef>
#include <QtGlobal>
#include <QVariant>

/*! \brief ID used by HeaderTableGraphicsItemMap
 */
class HeaderTableGraphicsItemMapId
{
 public:

  /*! \brief Get the value of this ID
   */
  constexpr
  std::size_t value() const noexcept
  {
    return mValue;
  }

  /*! \brief Get the value of this ID as a QVariant
   */
  QVariant toQVariant() const noexcept;

  /*! \brief Construct a ID from given value
   */
  static
  constexpr
  HeaderTableGraphicsItemMapId fromValue(std::size_t value) noexcept
  {
    return HeaderTableGraphicsItemMapId(value);
  }

  /*! \brief Check if given value is a valid QVariant
   */
  static
  bool isValidQVariant(const QVariant & value) noexcept;

  /*! \brief Construct a ID from given value
   *
   * \pre \a value must be a valid QVariant
   * \sa isValidQVariant()
   */
  static
  HeaderTableGraphicsItemMapId fromQVariant(const QVariant & value) noexcept;

 private:

  constexpr
  HeaderTableGraphicsItemMapId(std::size_t value) noexcept
   : mValue(value)
  {
  }

  std::size_t mValue;
};

#endif // #ifndef HEADER_TABLE_GRAPHICS_ITEM_MAP_ID_H

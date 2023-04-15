// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef NUMERIC_H
#define NUMERIC_H

#include <QtGlobal>
#include <cstdint>

/*! \brief Get a qulonglong out from a uint64_t
 *
 * \sa https://doc.qt.io/qt-6/qttypes.html#qulonglong-typedef
 */
inline
constexpr
qulonglong qulonglong_from_uint64(uint64_t u) noexcept
{
  return u;
}

/*! \brief Get a qreal out from a uint64_t
 *
 * \sa https://doc.qt.io/qt-6/qttypes.html#qreal-typedef
 */
inline
constexpr
qreal qreal_from_uint64(uint64_t u) noexcept
{
  return static_cast<qreal>(u);
}

#endif // #ifndef NUMERIC_H

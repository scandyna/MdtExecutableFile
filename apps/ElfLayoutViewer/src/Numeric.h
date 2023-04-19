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
#include <cstddef>
#include <limits>

/*! \brief Check if an int can represent given value of type std::size_t
 */
inline
constexpr
bool int_canHoldValueOf_size_t(std::size_t s) noexcept
{
  return s <= std::numeric_limits<int>::max();
}

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

/*! \brief Get a qulonglong out from a std::size_t
 *
 * \sa https://doc.qt.io/qt-6/qttypes.html#qulonglong-typedef
 */
inline
constexpr
qulonglong qulonglong_from_size_t(std::size_t s) noexcept
{
  return s;
}

/*! \brief Get a std::size_t out from a qulonglong
 *
 * \sa https://doc.qt.io/qt-6/qttypes.html#qulonglong-typedef
 */
inline
constexpr
std::size_t size_t_from_qulonglong(qulonglong u) noexcept
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

/*! \brief Get a qreal out from a uint64_t
 *
 * \sa https://doc.qt.io/qt-6/qttypes.html#qreal-typedef
 * \sa https://doc.qt.io/qt-6/qttypes.html#qulonglong-typedef
 */
inline
constexpr
qreal qreal_from_qulonglong(qulonglong u) noexcept
{
  return static_cast<qreal>(u);
}

#endif // #ifndef NUMERIC_H

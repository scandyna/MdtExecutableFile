// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_READER_UTILS_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_READER_UTILS_H

#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "Mdt/ExecutableFile/NotNullTerminatedStringError.h"
#include <QtEndian>
#include <QString>
#include <QCoreApplication>
#include <initializer_list>
#include <cstdint>
#include <algorithm>
#include <string>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

  /*! \internal
   */
  inline
  QString tr(const char *sourceText) noexcept
  {
    assert( sourceText != nullptr );

    return QCoreApplication::translate("Mdt::ExecutableFile::ExecutableFileReaderUtils", sourceText);
  }

  /*! \internal Get a string from a array of unsigned characters
   *
   * Will read from \a charArray until a null char is encountered,
   * or until the end of the array.
   * This means that this function accepts non null terminated strings.
   *
   * \pre \a charArray must not be null
   */
  inline
  std::string stringFromBoundedUnsignedCharArray(const ByteArraySpan & charArray) noexcept
  {
    assert( !charArray.isNull() );
    assert( charArray.size >= 1 );

    if( charArray.data[charArray.size-1] != 0 ){
      return std::string( reinterpret_cast<const char*>(charArray.data), static_cast<size_t>(charArray.size) );
    }

    return std::string( reinterpret_cast<const char*>(charArray.data) );
  }

  /*! \internal Check if \a charArray contains the end of string
   *
   * \pre \a charArray must not be null
   */
  inline
  bool containsEndOfString(const ByteArraySpan & charArray) noexcept
  {
    assert( !charArray.isNull() );

    const auto first = charArray.data;
    const auto last = charArray.data + charArray.size;

    return std::find(first, last, 0) != last;
  }

  /*! \internal Get a string from a array of unsigned characters
   *
   * \pre \a charArray must not be null
   * \exception NotNullTerminatedStringError
   */
  inline
  QString qStringFromUft8UnsignedCharArray(const ByteArraySpan & charArray)
  {
    assert( !charArray.isNull() );

    if( !containsEndOfString(charArray) ){
      const QString message = tr("failed to extract a string from a region (end of string not found)");
      throw NotNullTerminatedStringError(message);
    }

    return QString::fromUtf8( reinterpret_cast<const char*>(charArray.data) );
  }

  /*! \internal
   *
   * \pre \a start must not be a nullptr
   * \pre \a count must be >= 0
   */
  inline
  bool arraysAreEqual(const unsigned char * const start, int64_t count, std::initializer_list<unsigned char> reference) noexcept
  {
    assert( start != nullptr );
    assert( count >= 0 );

    const auto first = start;
    const auto last = start + count;

    return std::equal( first, last, reference.begin(), reference.end() );
  }

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array size must be at least 2 bytes
   */
  inline
  uint16_t get16BitValueLE(const ByteArraySpan & array) noexcept
  {
    assert( !array.isNull() );
    assert( array.size >= 2 );

    return qFromLittleEndian<uint16_t>(array.data);
  }

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array size must be at least 4 bytes
   */
  inline
  uint32_t get32BitValueLE(const ByteArraySpan & array) noexcept
  {
    assert( !array.isNull() );
    assert( array.size >= 4 );

    return qFromLittleEndian<uint32_t>(array.data);
  }

  /*! \internal
   *
   * \pre \a array must not be null
   * \pre \a array size must be at least 8 bytes
   */
  inline
  uint64_t get64BitValueLE(const ByteArraySpan & array) noexcept
  {
    assert( !array.isNull() );
    assert( array.size >= 8 );

    return qFromLittleEndian<uint64_t>(array.data);
  }

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_READER_UTILS_H

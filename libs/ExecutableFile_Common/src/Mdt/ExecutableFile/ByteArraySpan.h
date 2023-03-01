// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_BYTE_ARRAY_SPAN_H
#define MDT_EXECUTABLE_FILE_BYTE_ARRAY_SPAN_H

#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

  /*! \internal
   */
  struct ByteArraySpan
  {
    /*! \brief STL iterator
     */
    using iterator = unsigned char*;

    /*! \brief STL const iterator
     */
    using const_iterator = const unsigned char*;

    unsigned char *data = nullptr;
    int64_t size = 0;

    constexpr
    inline
    bool isNull() const noexcept
    {
      return data == nullptr;
    }

    /*! \brief Check if \a offset and \a count represents a valid range for this span
     *
     * \pre this span must not be null
     * \pre \a offset must be >= 0
     * \pre \a count must be > 0
     */
    constexpr
    inline
    bool isInRange(int64_t offset, int64_t count) const noexcept
    {
      assert( !isNull() );
      assert( offset >= 0 );
      assert( count > 0 );

      return (offset + count) <= size;
    }

    /*! \brief Get a span that is a view over the \a count elements of this span starting at \a offset
     *
     * \pre this span must not be null
     * \sa isNull()
     * \pre \a offset must be >= 0
     * \pre \a count must be > 0
     * \pre \a offset + \a count must be in range of this span
     * \sa isInRange()
     */
    constexpr
    ByteArraySpan subSpan(int64_t offset, int64_t count) const noexcept
    {
      assert( !isNull() );
      assert( offset >= 0 );
      assert( count > 0 );
      assert( isInRange(offset, count) );

      ByteArraySpan span;

      span.data = data + offset;
      span.size = count;

      return span;
    }

    /*! \brief Get a span that is a view over the remainding elements of this span starting at \a offset
     *
     * \pre this span must not be null
     * \pre \a offset must be >= 0
     * \pre \a offset must be < the size of this span
     */
    constexpr
    ByteArraySpan subSpan(int64_t offset) const noexcept
    {
      assert( !isNull() );
      assert( offset >= 0 );
      assert( offset < size );

      return subSpan(offset, size-offset);
    }

    /*! \brief Get a iterator to the beginning of this span
     */
    constexpr
    iterator begin() const noexcept
    {
      return data;
    }

    /*! \brief Get a iterator past the end of this span
     */
    constexpr
    iterator end() const noexcept
    {
      return data + size;
    }

    /*! \brief Get a iterator to the beginning of this span
     */
    constexpr
    const_iterator cbegin() const noexcept
    {
      return data;
    }

    /*! \brief Get a iterator past the end of this span
     */
    constexpr
    const_iterator cend() const noexcept
    {
      return data + size;
    }
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_BYTE_ARRAY_SPAN_H

// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ByteArraySpanTestUtils.h"
#include <QString>
#include <iostream>
#include <cassert>

using namespace Mdt::ExecutableFile;

ByteArraySpan arraySpanFromArray(unsigned char * const array, int64_t size)
{
  assert(size > 0);

  ByteArraySpan span;
  span.data = array;
  span.size = size;

  return span;
}

bool arraysAreEqual(const ByteArraySpan & array, const ByteArraySpan & reference)
{
  if(array.size != reference.size){
    std::cout << "sizes differ: " << QString::number(array.size).toStdString()
              << " , expected: " << QString::number(reference.size).toStdString() << std::endl;
    return false;
  }

  for(int64_t i = 0; i < array.size; ++i){
    if(array.data[i] != reference.data[i]){
      std::cout << "values differ at index " << i
      << ": 0x" << QString::number(array.data[i], 16).toStdString()
      << " , extected: 0x" << QString::number(reference.data[i], 16).toStdString() << std::endl;
      return false;
    }
  }

  return true;
//   return std::equal( array.cbegin(), array.cend(), reference.cbegin(), reference.cend() );
}

bool arraysAreEqual(const ByteArraySpan & array, std::vector<unsigned char> reference)
{
  ByteArraySpan referenceArray;
  referenceArray.data = reference.data();
  referenceArray.size = static_cast<int64_t>( reference.size() );

  return arraysAreEqual(array, referenceArray);
}

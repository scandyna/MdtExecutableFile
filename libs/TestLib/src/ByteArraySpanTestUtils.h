// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef BYTE_ARRAY_SPAN_TEST_UTILS_H
#define BYTE_ARRAY_SPAN_TEST_UTILS_H

#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include <vector>
#include <cstdint>

Mdt::ExecutableFile::ByteArraySpan arraySpanFromArray(unsigned char * const array, int64_t size);

bool arraysAreEqual(const Mdt::ExecutableFile::ByteArraySpan & array, const Mdt::ExecutableFile::ByteArraySpan & reference);

bool arraysAreEqual(const Mdt::ExecutableFile::ByteArraySpan & array, std::vector<unsigned char> reference);

#endif // #ifndef BYTE_ARRAY_SPAN_TEST_UTILS_H

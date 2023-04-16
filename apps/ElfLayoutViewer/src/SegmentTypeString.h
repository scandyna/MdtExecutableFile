// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SEGMENT_TYPE_STRING_H
#define SEGMENT_TYPE_STRING_H

#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <QString>

/*! \brief Get a short name of given segment type
 */
QString segmentTypeShortName(Mdt::ExecutableFile::Elf::SegmentType type) noexcept;

/*! \brief Get a user friendly name of given segment type
 */
QString segmentTypeUserFriendlyName(Mdt::ExecutableFile::Elf::SegmentType type) noexcept;

#endif // #ifndef SEGMENT_TYPE_STRING_H

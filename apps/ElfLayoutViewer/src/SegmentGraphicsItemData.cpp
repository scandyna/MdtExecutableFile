// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SegmentGraphicsItemData.h"
#include "SegmentTypeString.h"

SegmentGraphicsItemData
SegmentGraphicsItemData::fromProgramHeader(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept
{
  SegmentGraphicsItemData data;

  data.setOffset(header.offset);
  data.setSize(header.filesz);
  data.mName = segmentTypeShortName( header.segmentType() );

  return data;
}

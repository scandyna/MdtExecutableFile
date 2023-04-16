// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "SegmentTypeString.h"
#include <QLatin1String>


QString segmentTypeShortName(Mdt::ExecutableFile::Elf::SegmentType type) noexcept
{
  using Mdt::ExecutableFile::Elf::SegmentType;

  switch(type){
    case SegmentType::Null:
      return QLatin1String("NULL");
    case SegmentType::Load:
      return QLatin1String("LOAD");
    case SegmentType::Dynamic:
      return QLatin1String("DYNAMIC");
    case SegmentType::Interpreter:
      return QLatin1String("INTERP");
    case SegmentType::Note:
      return QLatin1String("NOTE");
    case SegmentType::ProgramHeaderTable:
      return QLatin1String("PHDR");
    case SegmentType::Tls:
      return QLatin1String("TLS");
    case SegmentType::Unknown:
      return QLatin1String("???");
    case SegmentType::GnuEhFrame:
      return QLatin1String("GNU_EH_FRAME");
    case SegmentType::GnuStack:
      return QLatin1String("GNU_STACK");
    case SegmentType::GnuRelRo:
      return QLatin1String("GNU_RELRO");
  }

  return QString();
}

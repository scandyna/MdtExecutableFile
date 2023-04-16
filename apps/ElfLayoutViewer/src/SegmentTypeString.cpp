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

//     Null = 0,                   /*!< PT_NULL: unused program header table entry */
//     Load = 0x01,                /*!< PT_LOAD: loadable segment */
//     Dynamic = 0x02,             /*!< PT_DYNAMIC: dynamic linking information */
//     Interpreter = 0x03,         /*!< PT_INTERP: interpreter */
//     Note = 0x04,                /*!< PT_NOTE: auxiliary information */
//     ProgramHeaderTable = 0x06,  /*!< PT_PHDR: program header table */
//     Tls = 0x07,                 /*!< PT_TLS: thread-Local Storage template */
//     Unknown = 0x10000000,       /*!< Not from the standard */
//     GnuEhFrame = 0x6474e550,    /*!< PT_GNU_EH_FRAME: the array element specifies the location and size
//                                       of the exception handling information as defined by the .eh_frame_hdr section. */
//     GnuStack = 0x6474e551,      /*!< PT_GNU_STACK: the p_flags member specifies the permissions on the segment
//                                      containing the stack and is used to indicate wether the stack should be executable.
//                                      The absense of this header indicates that the stack will be executable. */
//     GnuRelRo = 0x6474e552        /*!< PT_GNU_RELRO: the array element specifies the location and size
//                                      of a segment which may be made read-only after relocation shave been processed. */


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

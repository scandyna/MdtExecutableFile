// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_DEBUG_H
#define MDT_EXECUTABLE_FILE_ELF_DEBUG_H

#include "FileHeader.h"
#include "ProgramHeader.h"
#include "ProgramHeaderTable.h"
#include "FileAllHeaders.h"
#include "SectionHeader.h"
#include "DynamicSection.h"
#include "SymbolTable.h"
#include "GlobalOffsetTable.h"
#include "ProgramInterpreterSection.h"
#include "GnuHashTable.h"
#include "NoteSection.h"
#include "NoteSectionTable.h"
#include "mdt_deployutilscore_export.h"
#include <QString>
#include <QLatin1String>
#include <vector>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  class StringTable;

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(Class c);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(DataFormat dataFormat);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(OsAbiType osAbiType);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const Ident & ident);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(ObjectFileType type);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(Machine machine);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const FileHeader & header);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(SegmentType type);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const ProgramHeader & header);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const ProgramHeaderTable & headers);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(SectionType type);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const SectionHeader & header);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const std::vector<SectionHeader> & headers);

  /** \internal Get the sections / segments mapping
   *
   * \code
   * 00 PT_PHDR
   * 01 PT_INTERP .interp
   * \endcode
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString sectionSegmentMappingToDebugString(const ProgramHeaderTable & programHeaderTable, const std::vector<SectionHeader> & sectionHeaderTable);

  /*! \internal
   */
  inline
  QString sectionSegmentMappingToDebugString(const FileAllHeaders & headers)
  {
    return sectionSegmentMappingToDebugString( headers.programHeaderTable(), headers.sectionHeaderTable() );
  }

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const StringTable & table);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(DynamicSectionTagType type);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const DynamicStruct & section, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const DynamicSection & section, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const SymbolTableEntry & entry, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const PartialSymbolTable & table, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const GlobalOffsetTableEntry & entry, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const GlobalOffsetTable & table, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const ProgramInterpreterSection & section);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const GnuHashTable & table);

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const NoteSection & section, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const NoteSectionTable & table, const QString & leftPad = QLatin1String("  "));

  /** \internal Print the file layout regarding all headers (File, Programm, Section)
   *
   * \code
   * from 0x0000 to 0x1111 : file header
   * from 0x1112 to 0x1133 : program header table
   * from 0x1134 to 0x2000 : XY segment type
   * from 0x2001 to 0x3000 : XY segment type
   * from 0x3001 to 0x4000 : .xy section
   * from 0x4001 to 0x5000 : .xy section
   * from 0x5001 to 0x6000 : section header table
   * \endcode
   */
  MDT_DEPLOYUTILSCORE_EXPORT
  QString fileLayoutToDebugString(const FileHeader & fileHeader, ProgramHeaderTable programHeaders, std::vector<SectionHeader> sectionHeaders);

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_DEBUG_H

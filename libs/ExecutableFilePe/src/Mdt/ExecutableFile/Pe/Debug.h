// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PE_DEBUG_H
#define MDT_EXECUTABLE_FILE_PE_DEBUG_H

#include "Mdt/ExecutableFile/Pe/FileHeader.h"
#include "Mdt/ExecutableFile/Pe/SectionHeader.h"
#include "Mdt/ExecutableFile/Pe/ImportDirectory.h"
// #include "mdt_deployutilscore_export.h"
#include <QString>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const DosHeader & header);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(MachineType type);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const CoffHeader & header);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(MagicType type);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const OptionalHeader & header);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const SectionHeader & header);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const ImportDirectory & directory, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const ImportDirectoryTable & directoryTable);

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const DelayLoadDirectory & directory, const QString & leftPad = QLatin1String("  "));

  /*! \internal
   */
//   MDT_DEPLOYUTILSCORE_EXPORT
  QString toDebugString(const DelayLoadTable & table);

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{

#endif // #ifndef MDT_EXECUTABLE_FILE_PE_DEBUG_H

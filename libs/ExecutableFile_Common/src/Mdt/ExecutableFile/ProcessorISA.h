// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PROCESSOR_ISA_H
#define MDT_EXECUTABLE_FILE_PROCESSOR_ISA_H

#include <QMetaType>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Processor ISA enum
   */
  enum class ProcessorISA
  {
    Unknown,  /*!< Unknown */
    X86_32,   /*!< X86 32 bit processor */
    X86_64    /*!< X86 64 bit processor */
  };

}} // namespace Mdt{ namespace ExecutableFile{
Q_DECLARE_METATYPE(Mdt::ExecutableFile::ProcessorISA)

#endif // #ifndef MDT_EXECUTABLE_FILE_PROCESSOR_ISA_H

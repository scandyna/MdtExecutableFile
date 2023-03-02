// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2015-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_OPERATING_SYSTEM_H
#define MDT_EXECUTABLE_FILE_OPERATING_SYSTEM_H

#include <QMetaType>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief OS enum
   */
  enum class OperatingSystem
  {
    Unknown,  /*!< Unknown OS */
    Linux,    /*!< Linux OS */
    Windows   /*!< Windows OS */
  };

  /*! \brief Check if given OS is valid
   */
  inline
  bool operatingSystemIsValid(OperatingSystem os) noexcept
  {
    return os != OperatingSystem::Unknown;
  }

}} // namespace Mdt{ namespace ExecutableFile{
Q_DECLARE_METATYPE(Mdt::ExecutableFile::OperatingSystem)

#endif // #ifndef MDT_EXECUTABLE_FILE_OPERATING_SYSTEM_H

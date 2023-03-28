// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PE_IMPORT_DIRECTORY_H
#define MDT_EXECUTABLE_FILE_PE_IMPORT_DIRECTORY_H

#include <QLatin1String>
#include <cstdint>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

  /*! \internal
   */
  struct ImportDirectory
  {
    uint32_t nameRVA = 0;

    /*! \brief Check if this directory is null
     *
     * This directory is null if all values are 0
     *
     * \sa https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#the-idata-section
     */
    bool isNull() const noexcept
    {
      return nameRVA == 0;
    }
  };

  /*! \internal
   */
  using ImportDirectoryTable = std::vector<ImportDirectory>;

  /*! \internal
   */
  struct DelayLoadDirectory
  {
    uint32_t attributes = 0;
    uint32_t nameRVA = 0;

    /*! \brief Check if this directory is null
     *
     * This directory is null if all values are 0
     *
     * \sa https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#the-idata-section
     */
    bool isNull() const noexcept
    {
      if(attributes != 0){
        return false;
      }
      if(nameRVA != 0){
        return false;
      }
      return true;
    }
  };

  /*! \internal
   */
  using DelayLoadTable = std::vector<DelayLoadDirectory>;

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{

#endif // #ifndef MDT_EXECUTABLE_FILE_PE_IMPORT_DIRECTORY_H

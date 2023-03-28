// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PE_SECTION_HEADER_H
#define MDT_EXECUTABLE_FILE_PE_SECTION_HEADER_H

#include <QString>
#include <QLatin1Char>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

  /*! \internal
   */
  struct SectionHeader
  {
    QString name;
    uint32_t virtualSize = 0;
    uint32_t virtualAddress = 0;
    uint32_t sizeOfRawData = 0;
    uint32_t pointerToRawData = 0;

    bool seemsValid() const noexcept
    {
      if( name.isEmpty() ){
        return false;
      }
      if( name[0] == QLatin1Char('/') ){
        return false;
      }
      if( virtualSize == 0 ){
        return false;
      }
      if( sizeOfRawData == 0 ){
        return false;
      }
      if( pointerToRawData == 0 ){
        return false;
      }
      if( virtualAddress < pointerToRawData ){
        return false;
      }

      return true;
    }

    /*! \brief Check if \a rva is in this section
     *
     * \pre this header must be valid
     */
    bool rvaIsInThisSection(uint32_t rva) const noexcept
    {
      assert( seemsValid() );

      if( rva < virtualAddress ){
        return false;
      }
      if( rva >= (virtualAddress+virtualSize) ){
        return false;
      }

      return true;
    }

    /*! \brief Check if \a rva is a valid address
     *
     * \pre this header must be valid
     */
    bool rvaIsValid(uint32_t rva) const noexcept
    {
      assert( seemsValid() );

      if( !rvaIsInThisSection(rva) ){
        return false;
      }

      return rva >= virtualAddress_PointerToRawData_Offset();
    }

    /*! \brief Get a file offset from \a rva
     *
     * \pre this header must be valid
     * \pre \a rva must be valid
     */
    int64_t rvaToFileOffset(uint32_t rva) const noexcept
    {
      assert( seemsValid() );
      assert( rvaIsValid(rva) );

      return rva - virtualAddress_PointerToRawData_Offset();
    }

   private:

    inline
    uint32_t virtualAddress_PointerToRawData_Offset() const noexcept
    {
      return virtualAddress - pointerToRawData;
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{

#endif // #ifndef MDT_EXECUTABLE_FILE_PE_SECTION_HEADER_H

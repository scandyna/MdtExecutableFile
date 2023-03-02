// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_IDENT_H
#define MDT_EXECUTABLE_FILE_ELF_IDENT_H

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  enum class Class : unsigned char
  {
    ClassNone = 0,  /*!< Unknown */
    Class32 = 1,    /*!< 32-bit */
    Class64 = 2     /*!< 64-bit */
  };

  /*! \internal
   */
  enum class DataFormat : unsigned char
  {
    DataNone = 0, /*!< Unknown data format */
    Data2LSB = 1, /*!< Two's complement, little-endian */
    Data2MSB = 2  /*!< Two's complement, big-endian */
  };

  /*! \internal
   *
   * \sa https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html#elfid
   */
  enum class OsAbiType
  {
    SystemV = 0x00,       /*!< UNIX System V ABI */
    Linux = 0x03,         /*!< */
    Unknown = 63,         /*!< Not from the standard */
    ArchitectureSpecific  /*!< Architecture-specific value range 64-255 */
  };

  /*! \internal
   */
  struct Ident
  {
    bool hasValidElfMagicNumber = false;
    Class _class = Class::ClassNone;
    DataFormat dataFormat = DataFormat::DataNone;
    unsigned char version = 0;
    unsigned char osabi;
    unsigned char abiversion;

    /*! \brief Returns the OS ABI deduced from osabi value
     */
    constexpr
    OsAbiType osAbiType() const noexcept
    {
      if( osabi == 0 ){
        return OsAbiType::SystemV;
      }
      if( osabi == 3 ){
        return OsAbiType::Linux;
      }
      if( osabi >= 64 ){
        return OsAbiType::ArchitectureSpecific;
      }

      return OsAbiType::Unknown;
    }

    /*! \brief Returns true if this ident is valid
     */
    constexpr
    bool isValid() const noexcept
    {
      if( !hasValidElfMagicNumber ){
        return false;
      }
      if( _class == Class::ClassNone ){
        return false;
      }
      if( dataFormat == DataFormat::DataNone ){
        return false;
      }
      if( version != 1 ){
        return false;
      }
      if( osAbiType() == OsAbiType::Unknown ){
        return false;
      }

      return true;
    }

    /*! \brief Clear this ident
     */
    constexpr
    void clear() noexcept
    {
      hasValidElfMagicNumber = false;
      _class = Class::ClassNone;
      dataFormat = DataFormat::DataNone;
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_IDENT_H

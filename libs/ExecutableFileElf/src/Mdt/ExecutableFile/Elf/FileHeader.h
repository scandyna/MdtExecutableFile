// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_H
#define MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_H

#include "Ident.h"
#include <QtGlobal>
#include <cstdint>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal e_type from the header
   */
  enum class ObjectFileType : uint16_t
  {
    None = 0x00,            /*!< An unknown type */
    RelocatableFile = 0x01, /*!< A relocatable file */
    ExecutableFile = 0x02,  /*!< An executable file */
    SharedObject = 0x03,    /*!< A shared object */
    CoreFile = 0x04,        /*!< A core file */
    Unknown = 0x1000        /*!< Not from the standard */
  };

  /*! \internal e_machine from the header
   */
  enum class Machine : uint16_t
  {
    None = 0x00,      /*!< No specific instruction set */
    X86 = 0x03,       /*!< x86 */
    X86_64 = 0x3E,    /*!< AMD x86-64 */
    Unknown = 0xFFFF  /*!< Not from the standard */
  };

  /*! \internal
   */
  struct FileHeader
  {
    Ident ident;
    uint16_t type = 0;
    uint16_t machine;
    uint32_t version;
    uint64_t entry;
    uint64_t phoff;
    uint64_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum = 0;
    uint16_t shentsize;
    uint16_t shnum = 0;
    uint16_t shstrndx = 0;

    /*! \brief
     */
    constexpr
    void setObjectFileType(ObjectFileType t) noexcept
    {
      type = static_cast<uint16_t>(t);
    }

    /*! \brief
     */
    constexpr
    ObjectFileType objectFileType() const noexcept
    {
      switch(type){
        case 0x00:
          return ObjectFileType::None;
        case 0x01:
          return ObjectFileType::RelocatableFile;
        case 0x02:
          return ObjectFileType::ExecutableFile;
        case 0x03:
          return ObjectFileType::SharedObject;
        case 0x04:
          return ObjectFileType::CoreFile;
      }

      return ObjectFileType::Unknown;
    }

    /*! \brief
     */
    constexpr
    Machine machineType() const noexcept
    {
      switch(machine){
        case 0x00:
          return Machine::None;
        case 0x03:
          return Machine::X86;
        case 0x3E:
          return Machine::X86_64;
      }

      return Machine::Unknown;
    }

    /*! \brief
     */
    constexpr
    void setMachineType(Machine m) noexcept
    {
      machine = static_cast<uint16_t>(m);
    }

    /*! \brief Get the page size
     *
     * This is extracted from GNU's Gold source code (abi_pagesize),
     * as patchelf does.
     *
     * \pre this file header must be valid
     */
    constexpr
    uint64_t pageSize() const noexcept
    {
      switch( machineType() ){
        case Machine::X86:
        case Machine::X86_64:
          return 0x1000;
        default:
          break;
      }

      return 0;
    }

    /*! \brief Return true if this file header seems valid
     */
    constexpr
    bool seemsValid() const noexcept
    {
      if( !ident.isValid() ){
        return false;
      }
      if( objectFileType() == ObjectFileType::None ){
        return false;
      }
      if( machineType() == Machine::None ){
        return false;
      }
      if( machineType() == Machine::Unknown ){
        return false;
      }
      if( version != 1 ){
        return false;
      }

      return true;
    }

    /*! \brief Clear this file header
     */
    constexpr
    void clear() noexcept
    {
      ident.clear();
      type = 0;
      machine = 0;
    }

    /*! \brief Get the minimum size to read all program headers
     */
    constexpr
    int64_t minimumSizeToReadAllProgramHeaders() const noexcept
    {
      return static_cast<int64_t>(phoff) + static_cast<int64_t>(phnum) * static_cast<int64_t>(phentsize);
    }

    /*! \brief Get the minimum size to read all section headers
     */
    constexpr
    int64_t minimumSizeToReadAllSectionHeaders() const noexcept
    {
      return static_cast<qint64>(shoff) + static_cast<qint64>(shnum) * static_cast<qint64>(shentsize);
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_FILE_HEADER_H

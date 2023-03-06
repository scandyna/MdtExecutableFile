// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_H
#define MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_H

#include <cstdint>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   *
   * \sa https://refspecs.linuxbase.org/elf/gabi4+/ch5.pheader.html#p_type
   * \sa https://refspecs.linuxbase.org/LSB_3.1.1/LSB-Core-generic/LSB-Core-generic.html#PROGHEADER
   */
  enum class SegmentType
  {
    Null = 0,                   /*!< PT_NULL: unused program header table entry */
    Load = 0x01,                /*!< PT_LOAD: loadable segment */
    Dynamic = 0x02,             /*!< PT_DYNAMIC: dynamic linking information */
    Interpreter = 0x03,         /*!< PT_INTERP: interpreter */
    Note = 0x04,                /*!< PT_NOTE: auxiliary information */
    ProgramHeaderTable = 0x06,  /*!< PT_PHDR: program header table */
    Tls = 0x07,                 /*!< PT_TLS: thread-Local Storage template */
    Unknown = 0x10000000,       /*!< Not from the standard */
    GnuEhFrame = 0x6474e550,    /*!< PT_GNU_EH_FRAME: the array element specifies the location and size
                                      of the exception handling information as defined by the .eh_frame_hdr section. */
    GnuStack = 0x6474e551,      /*!< PT_GNU_STACK: the p_flags member specifies the permissions on the segment
                                     containing the stack and is used to indicate wether the stack should be executable.
                                     The absense of this header indicates that the stack will be executable. */
    GnuRelRo = 0x6474e552        /*!< PT_GNU_RELRO: the array element specifies the location and size
                                     of a segment which may be made read-only after relocation shave been processed. */
  };

  /*! \internal
   *
   * \sa SegmentPermissions
   */
  enum class SegmentPermission : uint32_t
  {
    None = 0,               /*!< All access denied */
    Execute = 0x01,         /*!< Execute */
    Write = 0x02,           /*!< Write */
    Read = 0x04,            /*!< Read */
    MaskProc =  0xf0000000  /*!< Unspecified: reserved for processor-specific semantics */
  };

  /*! \internal
   *
   * \sa SegmentPermission
   * \note We cannot use QFlags, because it can't be initialzed from a uint32_t
   */
  class SegmentPermissions
  {
   public:

    constexpr
    SegmentPermissions() noexcept = default;

    constexpr
    SegmentPermissions(SegmentPermission p) noexcept
    {
      mFlags = static_cast<uint32_t>(p);
    }

    constexpr
    uint32_t toRawFlags() const noexcept
    {
      return mFlags;
    }

    static
    constexpr
    SegmentPermissions fromRawFlags(uint32_t flags) noexcept
    {
      SegmentPermissions permissions;

      permissions.mFlags = flags;

      return permissions;
    }

   private:

    uint32_t mFlags = 0;
  };

  /*! \internal
   */
  inline
  constexpr
  SegmentPermissions operator|(SegmentPermission a, SegmentPermission b) noexcept
  {
    return SegmentPermissions::fromRawFlags( static_cast<uint32_t>(a) | static_cast<uint32_t>(b) );
  }

  /*! \internal
   *
   * offset: file offset to the first byte of the segment
   *
   * vaddr: virtual address in memory to the the first byte of the segment
   *
   * align:
   *
   * \todo see: https://stackoverflow.com/questions/42599558/elf-program-header-virtual-address-and-file-offset
   */
  struct ProgramHeader
  {
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;
    uint64_t memsz;
    uint64_t align;

    constexpr
    SegmentType segmentType() const noexcept
    {
      switch(type){
        case 0:
          return SegmentType::Null;
        case 0x01:
          return SegmentType::Load;
        case 0x02:
          return SegmentType::Dynamic;
        case 0x03:
          return SegmentType::Interpreter;
        case 0x04:
          return SegmentType::Note;
        case 0x06:
          return SegmentType::ProgramHeaderTable;
        case 0x07:
          return SegmentType::Tls;
        case 0x6474e550:
          return SegmentType::GnuEhFrame;
        case 0x6474e551:
          return SegmentType::GnuStack;
        case 0x6474e552:
          return SegmentType::GnuRelRo;

      }

      return SegmentType::Unknown;
    }

    /*! \brief Set the segment type
     */
    constexpr
    void setSegmentType(SegmentType t) noexcept
    {
      type = static_cast<uint32_t>(t);
    }

    /*! \brief Set the permissions to the segment represented by this header
     */
    constexpr
    void setPermissions(SegmentPermissions permissions) noexcept
    {
      flags = permissions.toRawFlags();
    }

    /*! \brief Check if the segment represented by this header is executable
     */
    constexpr
    bool isExecutable() const noexcept
    {
      return flags & static_cast<uint32_t>(SegmentPermission::Execute);
    }

    /*! \brief Check if the segment represented by this header is writable
     */
    constexpr
    bool isWritable() const noexcept
    {
      return flags & static_cast<uint32_t>(SegmentPermission::Write);
    }

    /*! \brief Check if the segment represented by this header is readable
     */
    constexpr
    bool isReadable() const noexcept
    {
      return flags & static_cast<uint32_t>(SegmentPermission::Read);
    }

    /*! \brief Check if the segment referred by this header requires alignment
     *
     * From the TIS ELF specification v1.2,
     * Book I, Section 2-2 "Program Header":
     *  If p_align has a value of 0 or 1, no alignment is required.
     */
    bool requiresAlignment() const noexcept
    {
      return align > 1;
    }

    /*! \brief Get the virtual address of the end of the segment represented by this header
     *
     * \note the returned address is 1 byte past the last virtual address of the segment
     */
    uint64_t segmentVirtualAddressEnd() const noexcept
    {
      return vaddr + memsz;
    }

    /*! \brief Get the file offset of the end of the segment represented by this header
     *
     * \note the returned offset is 1 byte past the last offset of the segment
     */
    uint64_t fileOffsetEnd() const noexcept
    {
      return offset + filesz;
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_PROGRAM_HEADER_H

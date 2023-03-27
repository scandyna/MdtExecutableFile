// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_PLATFORM_H
#define MDT_EXECUTABLE_FILE_PLATFORM_H

#include "Mdt/ExecutableFile/OperatingSystem.h"
#include "Mdt/ExecutableFile/Compiler.h"
#include "Mdt/ExecutableFile/ProcessorISA.h"
#include "Mdt/ExecutableFile/ExecutableFileFormat.h"
#include "mdt_executablefile_common_export.h"

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Definition of a platform
   *
   * In term of a executable or a shared library,
   * there are several parts of a platorm that can be interresting:
   * - The operating system on which the file is executed
   * - The executable file format (ELF, PE)
   * - The compiler that created the file
   * - The processor ISA on which the file is executed
   *
   * Note that the operating system and the executable file format
   * can seem redoundant. But, for example, ELF supports Linux, NetBSD, HP-UX and many others.
   */
  class MDT_EXECUTABLEFILE_COMMON_EXPORT Platform
  {
   public:

    /*! \brief Construct a null platform
     */
    Platform() noexcept = default;

    /*! \brief Construct a platform
     */
    Platform(OperatingSystem os, ExecutableFileFormat format, Compiler compiler, ProcessorISA processor) noexcept
     : mOperatingSystem(os),
       mExecutableFileFormat(format),
       mCompiler(compiler),
                     mProcessorISA(processor)
    {
    }

    /*! \brief Copy construct a platform from \a other
     */
    Platform(const Platform & other) noexcept = default;

    /*! \brief Copy assign \a other to this platform
     */
    Platform & operator=(const Platform & other) noexcept = default;

    /*! \brief Move construct a platform from \a other
     */
    Platform(Platform && other) noexcept = default;

    /*! \brief Move assign \a other to this platform
     */
    Platform & operator=(Platform && other) noexcept = default;

    /*! \brief Get operating system
     */
    OperatingSystem operatingSystem() const noexcept
    {
      return mOperatingSystem;
    }

    /*! \brief Get executable file format
     */
    ExecutableFileFormat executableFileFormat() const noexcept
    {
      return mExecutableFileFormat;
    }

    /*! \brief Get processor ISA
     */
    ProcessorISA processorISA() const noexcept
    {
      return mProcessorISA;
    }

    /*! \brief Get compiler
     */
    Compiler compiler() const noexcept
    {
      return mCompiler;
    }

    /*! \brief Check if this platform is null
     */
    bool isNull() const noexcept
    {
      if( mOperatingSystem == OperatingSystem::Unknown ){
        return true;
      }
      if( mExecutableFileFormat == ExecutableFileFormat::Unknown ){
        return true;
      }
      if( mCompiler == Compiler::Unknown ){
        return true;
      }
      if( mProcessorISA == ProcessorISA::Unknown ){
        return true;
      }

      return false;
    }

    /*! \brief Check if this platform supports rpath
     */
    bool supportsRPath() const noexcept
    {
      switch(mExecutableFileFormat){
        case ExecutableFileFormat::Elf:
          return true;
        case ExecutableFileFormat::Pe:
        case ExecutableFileFormat::Unknown:
          break;
      }

      return false;
    }

    /*! \brief Check if platform a and be are equal
     *
     * \note the compiler is not taken into account here.
     * One reason is that binaries having C ABI,
     * are compatible accross compilers (as far as I know).
     * Also, for C++ things can get a lot more complex.
     * What about the version of the compiler,
     * the standard library, the C++ version, and probably other stuff.
     * Also, it seems not reasonably possible to extract such information from a binary file.
     * Also, we should trust compilers/linkers to give correct dependencies,
     * they are required to be correct to execute anyway.
     * To have a overview of the problem, see https://conan.io
     *
     * \todo Maybe we have to remove Compiler from this class
     */
    friend
    bool operator==(const Platform & a, const Platform & b) noexcept
    {
      if( a.operatingSystem() != b.operatingSystem() ){
        return false;
      }
      if( a.executableFileFormat() != b.executableFileFormat() ){
        return false;
      }
      if( a.processorISA() != b.processorISA() ){
        return false;
      }
      return true;
    }

    /*! \brief Check if platform a and be are different
     *
     * \sa operator==()
     */
    friend
    bool operator!=(const Platform & a, const Platform & b) noexcept
    {
      return !(a == b);
    }

    /*! \brief Get native operating system
     */
    static
    OperatingSystem nativeOperatingSystem() noexcept;

    /*! \brief Get the native executable file format
     */
    static
    ExecutableFileFormat nativeExecutableFileFormat() noexcept;

    /*! \brief Get native compiler
     */
    static
    Compiler nativeCompiler() noexcept;


    /*! \brief Get native processor
     */
    static
    ProcessorISA nativeProcessorISA() noexcept;

    /*! \brief Get native platform
     */
    static
    Platform nativePlatform() noexcept
    {
      return Platform( nativeOperatingSystem(), nativeExecutableFileFormat(), nativeCompiler(), nativeProcessorISA() );
    }

   private:

    OperatingSystem mOperatingSystem = OperatingSystem::Unknown;
    ExecutableFileFormat mExecutableFileFormat = ExecutableFileFormat::Unknown;
    Compiler mCompiler = Compiler::Unknown;
    ProcessorISA mProcessorISA = ProcessorISA::Unknown;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_PLATFORM_H

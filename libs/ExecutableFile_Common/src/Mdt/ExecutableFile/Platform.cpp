// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Platform.h"
#include <QtGlobal>

namespace Mdt{ namespace ExecutableFile{

// Platform::Platform(OperatingSystem os, Compiler compiler, Processor processor) noexcept
//  : mOperatingSystem(os),
//    mCompiler(compiler),
//    mProcessor(processor)
// {
// }

OperatingSystem Platform::nativeOperatingSystem() noexcept
{
#ifdef Q_OS_LINUX
  return OperatingSystem::Linux;
#elif defined Q_OS_WIN
  return OperatingSystem::Windows;
#else
 #error "Current OS is not supported"
#endif // OS
}

ExecutableFileFormat Platform::nativeExecutableFileFormat() noexcept
{
#ifdef Q_OS_LINUX
  return ExecutableFileFormat::Elf;
#elif defined Q_OS_WIN
  return ExecutableFileFormat::Pe;
#else
 #error "Current OS is not supported"
#endif // OS
}

Compiler Platform::nativeCompiler() noexcept
{
#ifdef Q_CC_GNU
  return Compiler::Gcc;
#elif defined Q_CC_CLANG
  return Compiler::Clang;
#elif defined Q_CC_MSVC
  return Compiler::Msvc;
#else
 #error "Current compiler is not supported"
#endif // Compiler
}

ProcessorISA Platform::nativeProcessorISA() noexcept
{
#ifdef Q_PROCESSOR_X86_32
  return ProcessorISA::X86_32;
#elif defined Q_PROCESSOR_X86_64
  return ProcessorISA::X86_64;
#else
 #error "Current processor is not supported"
#endif // Compiler
}


}} // namespace Mdt{ namespace ExecutableFile{

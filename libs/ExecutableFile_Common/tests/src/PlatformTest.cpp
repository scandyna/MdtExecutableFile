// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ExecutableFile/Platform.h"

using namespace Mdt::ExecutableFile;

TEST_CASE("simpleSetGetTest")
{
  Platform pf(OperatingSystem::Windows, ExecutableFileFormat::Pe, Compiler::Gcc, ProcessorISA::X86_32);
  REQUIRE( pf.operatingSystem() == OperatingSystem::Windows );
  REQUIRE( pf.executableFileFormat() == ExecutableFileFormat::Pe );
  REQUIRE( pf.compiler() == Compiler::Gcc );
  REQUIRE( pf.processorISA() == ProcessorISA::X86_32 );
}

TEST_CASE("isNull")
{
  SECTION("default constructed")
  {
    Platform pf;
    REQUIRE( pf.isNull() );
  }

  SECTION("Valid")
  {
    Platform pf(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( !pf.isNull() );
  }
}

TEST_CASE("supportsRPath")
{
  SECTION("Null")
  {
    Platform pf;
    REQUIRE( pf.isNull() );
    REQUIRE( !pf.supportsRPath() );
  }

  SECTION("Linux")
  {
    Platform pf(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( pf.supportsRPath() );
  }

  SECTION("Windows")
  {
    Platform pf(OperatingSystem::Windows, ExecutableFileFormat::Pe, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( !pf.supportsRPath() );
  }
}

TEST_CASE("nativePlatformTest")
{
  Platform pf = Platform::nativePlatform();

  // Check that the correct OS was choosen
#ifdef Q_OS_LINUX
  REQUIRE(pf.operatingSystem() == OperatingSystem::Linux);
  REQUIRE(pf.executableFileFormat() == ExecutableFileFormat::Elf);
#elif defined Q_OS_WIN
  REQUIRE(pf.operatingSystem() == OperatingSystem::Windows);
  REQUIRE(pf.executableFileFormat() == ExecutableFileFormat::Pe);
#else
 #error "Current OS is not supported"
#endif

  // Check that correct compiler was choosen
#ifdef Q_CC_GNU
  REQUIRE(pf.compiler() == Compiler::Gcc);
#elif defined Q_CC_CLANG
  REQUIRE(pf.compiler() == Compiler::Clang);
#elif defined Q_CC_MSVC
  REQUIRE(pf.compiler() == Compiler::Msvc);
#else
 #error "Current compiler is not supported"
#endif

  // Check that correct processor was choosen
#ifdef Q_PROCESSOR_X86_32
  REQUIRE(pf.processorISA() == ProcessorISA::X86_32);
#elif defined Q_PROCESSOR_X86_64
  REQUIRE(pf.processorISA() == ProcessorISA::X86_64);
#else
 #error "Current processor is not supported"
#endif
}

TEST_CASE("comparison")
{
  SECTION("all matches")
  {
    Platform a(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    Platform b = a;
    REQUIRE( a == b );
    REQUIRE( !(a != b) );
  }

  SECTION("OS different")
  {
    Platform a(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    Platform b(OperatingSystem::Windows, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( !(a == b) );
    REQUIRE( a != b );
  }

  SECTION("executable file format differs")
  {
    Platform a(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    Platform b(OperatingSystem::Linux, ExecutableFileFormat::Pe, Compiler::Gcc, ProcessorISA::X86_64);
    REQUIRE( a != b );
  }

  SECTION("processor ISA differs")
  {
    Platform a(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    Platform b(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_32);
    REQUIRE( a != b );
  }

  SECTION("compiler differs (see remarks in the class documentation)")
  {
    Platform a(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Gcc, ProcessorISA::X86_64);
    Platform b(OperatingSystem::Linux, ExecutableFileFormat::Elf, Compiler::Clang, ProcessorISA::X86_64);
    REQUIRE( a == b );
  }
}

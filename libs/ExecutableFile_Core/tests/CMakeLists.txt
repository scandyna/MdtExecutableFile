# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)


mdt_add_test(
  NAME ElfFileIoEngineTest
  TARGET elfFileIoEngineTest
  DEPENDENCIES Mdt::ExecutableFileCore TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileIoEngineTest.cpp
)

if( UNIX AND (NOT APPLE) )

  # Uses project compiled test binaries
  mdt_add_test(
    NAME ElfFileIoEngineTest_Unix
    TARGET elfFileIoEngineTest_Unix
    DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
    SOURCE_FILES
      src/ElfFileIoEngineTest_Unix.cpp
  )

endif()

mdt_add_test(
  NAME ElfFileIoEngineErrorTest
  TARGET elfFileIoEngineErrorTest
  DEPENDENCIES Mdt::ExecutableFileCore Mdt::ExecutableFileElf TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ElfFileIoEngineErrorTest.cpp
)


mdt_add_test(
  NAME PeFileIoEngineTest
  TARGET peFileIoEngineTest
  DEPENDENCIES Mdt::ExecutableFileCore TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileIoEngineTest.cpp
)

if(WIN32)

  # Uses project compiled test binaries
  mdt_add_test(
    NAME PeFileIoEngineTest_Windows
    TARGET peFileIoEngineTest_Windows
    DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
    SOURCE_FILES
      src/PeFileIoEngineTest_Windows.cpp
  )

endif()

mdt_add_test(
  NAME PeFileIoEngineErrorTest
  TARGET peFileIoEngineErrorTest
  DEPENDENCIES Mdt::ExecutableFileCore Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileIoEngineErrorTest.cpp
)


mdt_add_test(
  NAME ExecutableFileIoEngineTest
  TARGET executableFileIoEngineTest
  DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileIoEngineTest.cpp
)

mdt_add_test(
  NAME ExecutableFileIoEngineErrorTest
  TARGET executableFileIoEngineErrorTest
  DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileIoEngineErrorTest.cpp
)

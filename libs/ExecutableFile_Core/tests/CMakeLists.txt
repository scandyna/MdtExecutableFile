# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)


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


mdt_add_test(
  NAME ExecutableFileReaderTest
  TARGET executableFileReaderTest
  DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileReaderTest.cpp
)
if(MSVC)
  target_compile_definitions(executableFileReaderTest PRIVATE COMPILER_IS_MSVC=1)
endif()

mdt_add_test(
  NAME ExecutableFileReaderErrorTest
  TARGET executableFileReaderErrorTest
  DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileReaderErrorTest.cpp
)

if(UNIX)
  mdt_add_test(
    NAME ExecutableFileReaderTest_Unix
    TARGET executableFileReaderTest_Unix
    DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
    SOURCE_FILES
      src/ExecutableFileReaderTest_Unix.cpp
  )
endif()


mdt_add_test(
  NAME ExecutableFileWriterTest
  TARGET executableFileWriterTest
  DEPENDENCIES Mdt::ExecutableFileCore TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileWriterTest.cpp
)
# target_compile_definitions(executableFileWriterTest PRIVATE TEST_SHARED_LIBRARY_FILE_PATH="$<TARGET_FILE:testSharedLibrary>")
# target_compile_definitions(executableFileWriterTest PRIVATE TEST_DYNAMIC_EXECUTABLE_FILE_PATH="$<TARGET_FILE:testExecutableDynamic>")

mdt_add_test(
  NAME ExecutableFileWriterErrorTest
  TARGET executableFileWriterErrorTest
  DEPENDENCIES Mdt::ExecutableFileCore Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileWriterErrorTest.cpp
)

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)


mdt_add_test(
  NAME PeFileReaderTest
  TARGET peFileReaderTest
  DEPENDENCIES Mdt::ExecutableFilePe Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileReaderTest.cpp
)

mdt_add_test(
  NAME PeFileIoEngineTest
  TARGET peFileIoEngineTest
  DEPENDENCIES Mdt::ExecutableFilePe TestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileIoEngineTest.cpp
)

if(WIN32)

  # Uses project compiled test binaries
  mdt_add_test(
    NAME PeFileIoEngineTest_Windows
    TARGET peFileIoEngineTest_Windows
    DEPENDENCIES Mdt::ExecutableFilePe TestBinariesUtils TestLib Mdt::Catch2Main Mdt::Catch2Qt
    SOURCE_FILES
      src/PeFileIoEngineTest_Windows.cpp
  )

endif()

mdt_add_test(
  NAME PeFileIoEngineErrorTest
  TARGET peFileIoEngineErrorTest
  DEPENDENCIES Mdt::ExecutableFilePe Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/PeFileIoEngineErrorTest.cpp
)

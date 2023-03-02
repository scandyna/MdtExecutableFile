# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(MdtAddTest)

mdt_add_test(
  NAME ExecutableFileIoEngineTest
  TARGET executableFileIoEngineTest
  DEPENDENCIES Mdt::ExecutableFileCore Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ExecutableFileIoEngineTest.cpp
)
# target_compile_definitions(executableFileIoEngineTest PRIVATE TEST_SHARED_LIBRARY_FILE_PATH="$<TARGET_FILE:testSharedLibrary>")
# target_compile_definitions(executableFileIoEngineTest PRIVATE TEST_DYNAMIC_EXECUTABLE_FILE_PATH="$<TARGET_FILE:testExecutableDynamic>")

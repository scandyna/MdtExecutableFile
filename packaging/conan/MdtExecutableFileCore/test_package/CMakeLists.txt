# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.15)

project(MdtExecutableFileCore_Test)

find_package(MdtCMakeModules REQUIRED)

include(MdtAddTest)
include(MdtRuntimeEnvironment)

enable_testing()


find_package(Mdt0 REQUIRED COMPONENTS ExecutableFileCore)
find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(executableFile_Test main.cpp)

target_link_libraries(executableFile_Test
  PRIVATE
    Mdt0::ExecutableFileCore Qt5::Core
)

add_test(NAME ExecutableFile_Test COMMAND executableFile_Test)
mdt_set_test_library_env_path(NAME ExecutableFile_Test TARGET executableFile_Test)

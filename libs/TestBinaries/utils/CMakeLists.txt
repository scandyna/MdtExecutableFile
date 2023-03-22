# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

# Using Conan with CMakeDeps breaks using $<TARGET_FILE:tgt>
# See https://gitlab.com/scandyna/mdt-cmake-modules/-/issues/12
if(WIN32)
  find_file(Qt5_CORE_SHARED_LIB_PATH Qt5Core.dll Qt5Cored.dll PATH_SUFFIXES "../../../bin" REQUIRED)
  find_file(Qt5_GUI_SHARED_LIB_PATH Qt5Gui.dll Qt5Guid.dll PATH_SUFFIXES "../../../bin" REQUIRED)
else()
  find_library(Qt5_CORE_SHARED_LIB_PATH libQt5Core.so.5 REQUIRED)
  find_library(Qt5_GUI_SHARED_LIB_PATH libQt5Gui.so.5 REQUIRED)
endif()

message(DEBUG "Qt5_CORE_SHARED_LIB_PATH: ${Qt5_CORE_SHARED_LIB_PATH}")
message(DEBUG "Qt5_GUI_SHARED_LIB_PATH: ${Qt5_GUI_SHARED_LIB_PATH}")


add_library(TestBinariesUtils STATIC
  TestBinariesUtils.cpp
)

target_include_directories(TestBinariesUtils
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(TestBinariesUtils
  PUBLIC
    Qt5::Core
    TestLib
)

target_compile_definitions(TestBinariesUtils
  PRIVATE
    TEST_SHARED_LIBRARY_FILE_PATH="$<TARGET_FILE:testSharedLibrary>"
    QT5_CORE_FILE_PATH="${Qt5_CORE_SHARED_LIB_PATH}"
#     TEST_STATIC_LIBRARY_FILE_PATH="$<TARGET_FILE:Mdt::ExecutableFile_Common>"
    TEST_DYNAMIC_EXECUTABLE_FILE_PATH="$<TARGET_FILE:testExecutableDynamic>"
)

add_dependencies(TestBinariesUtils
  testSharedLibrary
  testExecutableDynamic
)

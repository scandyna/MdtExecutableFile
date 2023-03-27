# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(GenerateExportHeader)
include(MdtInstallLibrary)

#
# First attempt was to make this library forced to be STATIC.
# This was a bad idea, because it exposes stuff to the public API.
# See https://gitlab.com/scandyna/mdtexecutablefile/-/issues/1
#
add_library(Mdt_ExecutableFile_Common
  Mdt/ExecutableFile/Algorithm.cpp
  Mdt/ExecutableFile/QRuntimeError.cpp
  Mdt/ExecutableFile/FileOpenError.cpp
  Mdt/ExecutableFile/ExecutableFileReadError.cpp
  Mdt/ExecutableFile/ExecutableFileWriteError.cpp
  Mdt/ExecutableFile/NotNullTerminatedStringError.cpp
  Mdt/ExecutableFile/StringTableError.cpp
  Mdt/ExecutableFile/Platform.cpp
  Mdt/ExecutableFile/ByteArraySpan.cpp
  Mdt/ExecutableFile/FileMapper.cpp
  Mdt/ExecutableFile/ExecutableFileReaderUtils.cpp
  Mdt/ExecutableFile/RPathFormatError.cpp
  Mdt/ExecutableFile/RPath.cpp
)

add_library(Mdt::ExecutableFile_Common ALIAS Mdt_ExecutableFile_Common)

target_compile_features(Mdt_ExecutableFile_Common PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFile_Common
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()


target_include_directories(Mdt_ExecutableFile_Common
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFile_Common
  PUBLIC
    Qt5::Core
#   PRIVATE
#     Boost::boost
)

generate_export_header(Mdt_ExecutableFile_Common)

mdt_install_library(
  TARGET Mdt_ExecutableFile_Common
  RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES_DIRECTORY .
  INCLUDES_FILE_WITHOUT_EXTENSION
  ADDITIONAL_INCLUDES_FILES "${CMAKE_CURRENT_BINARY_DIR}/mdt_executablefile_common_export.h"
  INCLUDES_DESTINATION ${MDT_INSTALL_INCLUDEDIR}
  EXPORT_NAME ExecutableFileCommon
  EXPORT_NAMESPACE Mdt${PROJECT_VERSION_MAJOR}::
  INSTALL_NAMESPACE ${MDT_INSTALL_PACKAGE_NAME}
  FIND_PACKAGE_PATHS ..
  INSTALL_IS_UNIX_SYSTEM_WIDE ${MDT_INSTALL_IS_UNIX_SYSTEM_WIDE}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION_COMPATIBILITY ExactVersion
  RUNTIME_COMPONENT MdtExecutableFileCore_Runtime
  DEVELOPMENT_COMPONENT MdtExecutableFileCore_Dev
)

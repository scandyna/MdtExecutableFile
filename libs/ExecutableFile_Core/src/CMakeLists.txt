# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(GenerateExportHeader)
include(MdtInstallLibrary)


add_library(Mdt_ExecutableFileCore
  Mdt/ExecutableFile/ExecutableFileIoEngine.cpp
  Mdt/ExecutableFile/ExecutableFileReader.cpp
  Mdt/ExecutableFile/ExecutableFileWriter.cpp
)
add_library(Mdt::ExecutableFileCore ALIAS Mdt_ExecutableFileCore)

target_compile_features(Mdt_ExecutableFileCore PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFileCore
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(Mdt_ExecutableFileCore
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFileCore
  PUBLIC
    Qt5::Core
    Mdt::ExecutableFile_Common
  PRIVATE
    Mdt::ExecutableFileElf
    Mdt::ExecutableFilePe
#     Boost::boost
)

generate_export_header(Mdt_ExecutableFileCore)

mdt_install_library(
  TARGET Mdt_ExecutableFileCore
  RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES_DIRECTORY .
  INCLUDES_FILE_WITHOUT_EXTENSION
  ADDITIONAL_INCLUDES_FILES "${CMAKE_CURRENT_BINARY_DIR}/mdt_executablefilecore_export.h"
  INCLUDES_DESTINATION ${MDT_INSTALL_INCLUDEDIR}
  EXPORT_NAME ExecutableFileCore
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

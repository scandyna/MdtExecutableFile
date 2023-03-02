# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.15)

# CMAKE_PREFIX_PATH is empty before any call to project()
# Because set_project_version() does not exist,
# we call the project command twice (is there a cleaner solution?)
project(MdtExecutableFile)

#######################################################
# Project definition with versionning got from git tag
#######################################################

find_package(Git REQUIRED)
find_package(MdtCMakeModules REQUIRED)

message(DEBUG "MDT_CMAKE_MODULES_PATH: ${MDT_CMAKE_MODULES_PATH}")

include(MdtVersionUtils)

set(INITIAL_PROJECT_VERSION)
if(FROM_CONAN_PROJECT_VERSION)
  set(INITIAL_PROJECT_VERSION ${FROM_CONAN_PROJECT_VERSION})
else()
  mdt_cmake_project_version_from_git_tag(INITIAL_PROJECT_VERSION DEFAULT_VERSION 0.0.0)
endif()

project(MdtExecutableFile VERSION ${INITIAL_PROJECT_VERSION} LANGUAGES CXX)

unset(INITIAL_PROJECT_VERSION)

#######################
# Options
#######################

include(MdtBuildOptionsUtils)
include(MdtSanitizers)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

option(BUILD_TESTS "Build the tests" OFF)
option(BUILD_BENCHMARKS "Build the benchmarks" OFF)
option(BUILD_DOCS "Build the documentations" OFF)
mdt_set_available_build_types(Release Debug RelWithDebInfo MinSizeRel Instrumented)
option(WARNING_AS_ERROR "Treat warnings as errors" OFF)

option(BUILD_USE_IPO_LTO_IF_AVAILABLE "Use link-time optimization if available on the platform" OFF)
mark_as_advanced(BUILD_USE_IPO_LTO_IF_AVAILABLE)
# Provide LPO/LTO option if supported
# Note: CMake before 3.9 does only support Intel compiler on Linux.
#       Check documentation of the CheckIPOSupported module,
#       and also CMP0069 policy.
include(CheckIPOSupported)
check_ipo_supported(RESULT HAVE_IPO_LTO)
if(HAVE_IPO_LTO)
  option(BUILD_USE_IPO_LTO "Use link-time optimization" ${BUILD_USE_IPO_LTO_IF_AVAILABLE})
endif()

mdt_get_available_optimization_levels(AVAILABLE_OPTIMIZATION_LEVELS)
set(BUILD_TYPE_INSTRUMENTED_OPTIMIZATION_LEVEL "" CACHE STRING "Set optimization level for Instrumented build")
set_property(CACHE BUILD_TYPE_INSTRUMENTED_OPTIMIZATION_LEVEL PROPERTY STRINGS ${AVAILABLE_OPTIMIZATION_LEVELS})
option(BUILD_TYPE_INSTRUMENTED_USE_DEBUG_SYMBOLS "Add debug symbols (-g on Gcc/Clang, /DEBUG on MSVC)" ON)
option(BUILD_TYPE_INSTRUMENTED_DEFINE_NDEBUG "Set NDEBUG definition for Instrumented build (will disable assert)" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(INSTALL_CONAN_PACKAGE_FILES "Install files required for recent conan generators, like CMakeDeps" OFF)

mdt_add_address_sanitizer_option_if_available(SANITIZER_ENABLE_ADDRESS
  HELP_STRING "Enable address sanitizer for Instrumented, RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_memory_sanitizer_option_if_available(SANITIZER_ENABLE_MEMORY
  HELP_STRING "Enable memory sanitizer for Instrumented, RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_undefined_sanitizer_option_if_available(SANITIZER_ENABLE_UNDEFINED
  HELP_STRING "Enable undefined behaviour sanitizer for Instrumented, RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_thread_sanitizer_option_if_available(SANITIZER_ENABLE_THREAD
  HELP_STRING "Enable thread sanitizer for Instrumented, RelWithDebInfo, Debug build (can be incompatible with other sanitizers)"
  INITIAL_VALUE OFF
)

#######################
# Warnings
#######################

if(MSVC)
  add_compile_options(/W3)
  if(WARNING_AS_ERROR)
    add_compile_options(/WX)
  endif()
  # C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead (which not works with gcc/clang)
  add_compile_options(/wd4996)
else()
  # TODO: check about -Wshadow
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wunused)
  if(WARNING_AS_ERROR)
    add_compile_options(-Werror)
  endif()
endif()

#######################
# Compilers issues
#######################

# GCC: mangled name for 'xx' will change in C++17 because the exception specification is part of a function type [-Wnoexcept-type]
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80985
if( (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0) )
  add_compile_options(-Wno-noexcept-type)
endif()

# https://gitlab.com/scandyna/mdtexecutablefile/-/issues/4
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  add_compile_options(-Wno-error=maybe-uninitialized)
endif()

# Workaround for the "too many sections" error on some MinGW compiler
# See: https://stackoverflow.com/questions/16596876/object-file-has-too-many-sections
if(MINGW)
  add_compile_options(-Wa,-mbig-obj)
endif()

#######################
# Paths to dependencies
#######################

include(AddQt5ToCMakePrefixPath)

set(QT_PREFIX_PATH CACHE PATH "Path to the root of Qt distribution. (For example: /opt/qt/Qt5/5.13.0/gcc_64). If empty, system distribution is considered.")
add_qt5_to_cmake_prefix_path("${QT_PREFIX_PATH}")

#######################
# Dependencies
#######################

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

# DeployUtils_Core uses Boost Graph
# For header only library,no component is defined.
# https://cmake.org/pipermail/cmake/2013-September/055941.html
find_package(Boost REQUIRED)

if(BUILD_TESTS)
  find_package(Catch2 REQUIRED)
  find_package(Qt5 COMPONENTS Test REQUIRED)
endif()

if(BUILD_BENCHMARKS)
  find_package(Qt5 COMPONENTS Test REQUIRED)
endif()

if(BUILD_DOCS)
  find_package(Sphinx REQUIRED)

  include(FindDoxygen)
  find_package(Doxygen REQUIRED dot)
endif()

#######################
# Sanitizers
#######################

if(SANITIZER_ENABLE_ADDRESS)
  mdt_build_with_address_sanitizer(BUILD_TYPES Instrumented RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_MEMORY)
  mdt_build_with_memory_sanitizer(BUILD_TYPES Instrumented RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_UNDEFINED)
  mdt_build_with_undefined_sanitizer(BUILD_TYPES Instrumented RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_THREAD)
  mdt_build_with_thread_sanitizer(BUILD_TYPES Instrumented RelWithDebInfo Debug)
endif()

#######################
# Instrumented build
#######################

if(BUILD_TYPE_INSTRUMENTED_OPTIMIZATION_LEVEL)
  add_compile_options($<$<CONFIG:Instrumented>:${BUILD_TYPE_INSTRUMENTED_OPTIMIZATION_LEVEL}>)
endif()

if(BUILD_TYPE_INSTRUMENTED_USE_DEBUG_SYMBOLS)
  mdt_add_debug_symbols_compile_option(BUILD_TYPES Instrumented)
endif()

if(BUILD_TYPE_INSTRUMENTED_DEFINE_NDEBUG)
  add_definitions($<$<CONFIG:Instrumented>:NDEBUG>)
endif()

#######################
# Install
#######################

set(MDT_INSTALL_PACKAGE_NAME Mdt${PROJECT_VERSION_MAJOR})
include(GNUInstallDirs)
include(MdtInstallDirs)
include(MdtPackageConfigHelpers)

#######################
# Qt
#######################

add_definitions(-DQT_NO_CAST_DEFINITIONS -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)

# TODO: enable before migrate to Qt6
# See https://doc.qt.io/qt-6/portingguide.html
# add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

# TODO: remove before migrate to Qt6
add_compile_definitions(QT_NO_DEPRECATED_WARNINGS=1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

###############################
# Binary outputs in build tree
###############################

# On Windows some tests fails because of missing dll's
# See https://gitlab.com/scandyna/mdt-cmake-modules/-/issues/4
if(WIN32)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/bin")
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/lib")
  endif()
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/lib")
  endif()
endif()

#######################
# Sources
#######################

if(BUILD_TESTS OR BUILD_BENCHMARKS)
  enable_testing()
  add_subdirectory(libs/Catch2)
  add_subdirectory(libs/TestLib)
endif()

add_subdirectory(libs/ExecutableFile_Common)
add_subdirectory(libs/ExecutableFile_Core)

if(BUILD_DOCS)
  add_subdirectory(doc)
endif()

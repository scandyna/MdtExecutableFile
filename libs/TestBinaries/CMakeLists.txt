# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.
include(ExternalData)

############################################
# Package registry URL
############################################

set(TEST_BINARIES_PACKAGE_BASE_URL "https://gitlab.com/api/v4/projects/40217884/packages/generic")
set(TEST_BINARIES_PACKAGE_VERSION "0.0.1")

############################################
# Fetch the binaries text files
# that contains the MD5 sum
# for each real binary that will be fetched
############################################

set(TEST_BINARIES_SOURCE_FILES
  linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_rpath_10_chars_Debug.md5
  lib_linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_no_rpath_Debug.so.md5
  lib_linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_rpath_10_chars_Debug.so.md5
  linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_no_rpath_Debug.md5
  linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_rpath_10_chars_Debug.md5
  linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_no_rpath_Debug.md5
  lib_linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_no_rpath_Debug.so.md5
)

option(FETCH_TEST_BINARIES_TEXT_FILES "Download the listed test binaries (will update the source tree)" OFF)
if(FETCH_TEST_BINARIES_TEXT_FILES)
  foreach(sourceFile ${TEST_BINARIES_SOURCE_FILES})
    message(STATUS "downloading ${sourceFile}")
    file(DOWNLOAD
      "${TEST_BINARIES_PACKAGE_BASE_URL}/src/${TEST_BINARIES_PACKAGE_VERSION}/${sourceFile}"
      "${CMAKE_CURRENT_SOURCE_DIR}/bin/${sourceFile}"
      SHOW_PROGRESS
    )
  endforeach()
endif()

unset(TEST_BINARIES_SOURCE_FILES)

############################################
# Add the package registry to ExternalData
# and create a target that represents
# the test binaries
############################################

set(ExternalData_URL_TEMPLATES
  "${TEST_BINARIES_PACKAGE_BASE_URL}/%(algo)/${TEST_BINARIES_PACKAGE_VERSION}/%(hash)"
)

ExternalData_Expand_Arguments(TestBinaries
  TEST_BINARIES_PATH_LIST
  DATA{bin/linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_rpath_10_chars_Debug}
  DATA{bin/lib_linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_no_rpath_Debug.so}
  DATA{bin/lib_linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_rpath_10_chars_Debug.so}
  DATA{bin/linux_ubuntu_18.04_gcc_8_ld_bfd_x86_64_no_rpath_Debug}
  DATA{bin/linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_rpath_10_chars_Debug}
  DATA{bin/linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_no_rpath_Debug}
  DATA{bin/lib_linux_ubuntu_18.04_gcc_8_ld_gold_x86_64_no_rpath_Debug.so}
)
ExternalData_Add_Target(TestBinaries)

# Extract the path to each binary
# and put them to some more meaningful variables

list(GET TEST_BINARIES_PATH_LIST 0 LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG)
list(GET TEST_BINARIES_PATH_LIST 1 LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG_SO)
list(GET TEST_BINARIES_PATH_LIST 2 LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG_SO)
list(GET TEST_BINARIES_PATH_LIST 3 LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG)
list(GET TEST_BINARIES_PATH_LIST 4 LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_RPATH_10_CHARS_DEBUG)
list(GET TEST_BINARIES_PATH_LIST 5 LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG)
list(GET TEST_BINARIES_PATH_LIST 6 LIB_LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG_SO)

unset(TEST_BINARIES_PATH_LIST)

set(LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG_BIN_PATH "${LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG}" PARENT_SCOPE)
set(LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG_SO_BIN_PATH "${LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG_SO}" PARENT_SCOPE)
set(LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG_SO_BIN_PATH "${LIB_LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_RPATH_10_CHARS_DEBUG_SO}" PARENT_SCOPE)
set(LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG_BIN_PATH "${LINUX_UBUNTU_18_04_GCC_8_LD_BFD_X86_64_NO_RPATH_DEBUG}" PARENT_SCOPE)
set(LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_RPATH_10_CHARS_DEBUG_BIN_PATH "${LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_RPATH_10_CHARS_DEBUG}" PARENT_SCOPE)
set(LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG_BIN_PATH "${LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG}" PARENT_SCOPE)
set(LIB_LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG_SO_BIN_PATH "${LIB_LINUX_UBUNTU_18_04_GCC_8_LD_GOLD_X86_64_NO_RPATH_DEBUG_SO}" PARENT_SCOPE)

#################################################
# Local binaries + helpers
#################################################
add_subdirectory(local)
add_subdirectory(utils)

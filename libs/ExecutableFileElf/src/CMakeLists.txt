# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(Mdt_ExecutableFileElf STATIC
  Mdt/ExecutableFile/Elf/Exceptions.cpp
  Mdt/ExecutableFile/Elf/Algorithm.cpp
  Mdt/ExecutableFile/Elf/Ident.cpp
  Mdt/ExecutableFile/Elf/FileHeader.cpp
  Mdt/ExecutableFile/Elf/SectionHeader.cpp
  Mdt/ExecutableFile/Elf/SectionIndexChangeMap.cpp
  Mdt/ExecutableFile/Elf/SectionHeaderTable.cpp
  Mdt/ExecutableFile/Elf/SectionHeaderReaderWriterCommon.cpp
  Mdt/ExecutableFile/Elf/StringTable.cpp
  Mdt/ExecutableFile/Elf/FileHeaderReaderWriterCommon.cpp
  Mdt/ExecutableFile/Elf/DynamicSection.cpp
  Mdt/ExecutableFile/Elf/NoteSection.cpp
  Mdt/ExecutableFile/Elf/NoteSectionTable.cpp
#   Mdt/ExecutableFile/Elf/NoteSectionReader.cpp
#   Mdt/ExecutableFile/Elf/NoteSectionWriter.cpp
  Mdt/ExecutableFile/Elf/ProgramInterpreterSection.cpp
  Mdt/ExecutableFile/Elf/HashTable.cpp
  Mdt/ExecutableFile/Elf/GnuHashTable.cpp
  Mdt/ExecutableFile/Elf/GlobalOffsetTable.cpp
#   Mdt/ExecutableFile/Elf/Debug.cpp
)

add_library(Mdt::ExecutableFileElf ALIAS Mdt_ExecutableFileElf)

target_compile_features(Mdt_ExecutableFileElf PUBLIC cxx_std_17)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ExecutableFileElf
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(Mdt_ExecutableFileElf
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ExecutableFileElf
  PUBLIC
    Qt5::Core
    Mdt::ExecutableFile_Common
#   PRIVATE
#     Boost::boost
)

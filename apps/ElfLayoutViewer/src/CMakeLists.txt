# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(ElfLayoutViewer STATIC
  Numeric.cpp
  SegmentTypeString.cpp
  LayoutGraphicsItem.cpp
  SectionGraphicsItemData.cpp
  SectionGraphicsItem.cpp
  SegmentGraphicsItemData.cpp
  SegmentGraphicsItem.cpp
  SectionHeaderTableModel.cpp
  ProgramHeaderTableModel.cpp
)
add_library(Mdt::ElfLayoutViewer ALIAS ElfLayoutViewer)

if(BUILD_USE_IPO_LTO)
  set_target_properties(ElfLayoutViewer
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

set_target_properties(ElfLayoutViewer
  PROPERTIES POSITION_INDEPENDENT_CODE TRUE
)

target_include_directories(ElfLayoutViewer
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(ElfLayoutViewer
  PUBLIC
    Qt5::Widgets
    Mdt::ExecutableFileElf
)


add_executable(mdtelflayoutviewer
  MainWindow.cpp
  main.cpp
)

if(BUILD_USE_IPO_LTO)
  set_target_properties(mdtelflayoutviewer
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(mdtelflayoutviewer
  PRIVATE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(mdtelflayoutviewer
  PRIVATE
    Mdt::ElfLayoutViewer
)

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_executable(mdtelflayoutview
  LayoutGraphicsItem.cpp
  SectionGraphicsItem.cpp
  SegmentGraphicsItem.cpp
  SectionHeaderTableModel.cpp
  ProgramHeaderTableModel.cpp
  MainWindow.cpp
  main.cpp
)

if(BUILD_USE_IPO_LTO)
  set_target_properties(mdtelflayoutview
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(mdtelflayoutview
  PRIVATE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(mdtelflayoutview
  PRIVATE
    Qt5::Widgets
    Mdt::ExecutableFileElf
)

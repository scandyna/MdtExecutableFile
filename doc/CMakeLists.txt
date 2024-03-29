# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

############################
# Variables that are reused
# in the Doxyfiles
############################

# set(DOT_FILES_CMAKE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dotfiles/cmake")

find_file(PLANTUML_JAR_PATH "plantuml.jar" PATHS "/usr/share/plantuml")
if(NOT EXISTS "${PLANTUML_JAR_PATH}")
  message(WARNING "plantuml.jar not found, UML diagrams will not be generated")
  set(PLANTUML_JAR_PATH "")
endif()

set(PLANTUML_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/doc/plantuml")

#######################
# C++ API doc
#######################

configure_file(
  "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
  "${PROJECT_BINARY_DIR}/doc/Doxyfile"
)

add_custom_command(
    OUTPUT cpp-api-doc
    COMMAND
      Doxygen::doxygen "${PROJECT_BINARY_DIR}/doc/Doxyfile"
    COMMENT "Build C++ API doc"
    VERBATIM
)
set_property(SOURCE cpp-api-doc PROPERTY SYMBOLIC 1)
add_custom_target(CppApiDoc ALL DEPENDS cpp-api-doc)


#######################
# C++ API doc - dev
#######################

configure_file(
  "${PROJECT_SOURCE_DIR}/doc/Doxyfile-dev.in"
  "${PROJECT_BINARY_DIR}/doc-dev/Doxyfile"
)

add_custom_command(
    OUTPUT cpp-api-doc-dev
    COMMAND
      Doxygen::doxygen "${PROJECT_BINARY_DIR}/doc-dev/Doxyfile"
    COMMENT "Build C++ API doc - dev"
    VERBATIM
)
set_property(SOURCE cpp-api-doc-dev PROPERTY SYMBOLIC 1)
add_custom_target(CppApiDoc-dev ALL DEPENDS cpp-api-doc-dev)

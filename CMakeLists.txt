if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

cmake_minimum_required (VERSION 2.6)
project (ALOE_PARENT)

# The version number.
set (ALOE_PARENT_VERSION_MAJOR 2)
set (ALOE_PARENT_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}/aloe_core/swapi/include/")
include_directories("${PROJECT_SOURCE_DIR}/aloe_core/swapi/include/aloe/")

set(MODULE_REPOS_NAME "default")

add_subdirectory(aloe_core)
add_subdirectory(modrep_default)
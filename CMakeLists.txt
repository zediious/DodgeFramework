cmake_minimum_required(VERSION 3.10...3.31.5)
set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION OFF)

project(
	DodgeFramework
	VERSION 1.1.0
	LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

macro(set_from_environment VARIABLE)
	if(NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif()
endmacro()

set_from_environment(CompiledPluginsPath)
if(NOT DEFINED CompiledPluginsPath)
	message(FATAL_ERROR "CompiledPluginsPath is not set")
endif()

option(COPY_OUTPUT "Copy the output of build operations to the game directory" ON)
option(ENABLE_SKYRIM_SE "Enable support for Skyrim SE in the dynamic runtime feature." ON)
option(ENABLE_SKYRIM_AE "Enable support for Skyrim AE in the dynamic runtime feature." ON)
option(ENABLE_SKYRIM_VR "Enable support for Skyrim VR in the dynamic runtime feature." OFF)
set(BUILD_TESTS OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(src)
include(cmake/packaging.cmake)

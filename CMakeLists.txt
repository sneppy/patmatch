cmake_minimum_required(VERSION 3.12)

## Project setup
project(patmatch C)

## External scripts
include (cmake/platform.cmake)

## Build type configuration
set(CMAKE_C_FLAGS_DEBUG				"-DBUILD_DEBUG=1 -ggdb -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO	"-DBUILD_DEVELOPMENT=1 -ggdb -O1 -foptimize-sibling-calls")
set(CMAKE_C_FLAGS_RELEASE			"-DBUILD_RELEASE=1 -Ofast -fno-tree-slp-vectorize")

## Compiler setup
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")

## Application setup
# Collect source files
file(GLOB_RECURSE SRC_FILES

	"src/*.c"
)
file(GLOB_RECURSE H_FILES

	"include/*.h"
)

# Create executable target
add_executable(${PROJECT_NAME}

	${SRC_FILES}
	${H_FILES}
)

# Set output dir
set(OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME}

	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

# Set include directories
target_include_directories(${PROJECT_NAME}

	PUBLIC
		"include"
)

## Custom makefile targets
add_custom_target(run

	COMMAND				${OUTPUT_DIR}/${PROJECT_NAME}
	DEPENDS				${PROJECT_NAME}
	WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}
)
cmake_minimum_required (VERSION 3.24)

project ("Simple CMake Template" VERSION 1.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Tell MSVC to build using multiple processes.
# This may speed up compilation time significantly.
# For more information check:
# https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes?view=msvc-170
#
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

# Makes it easier to display some useful info
include(CMakePrintHelpers)

# Uncomment the line below, if you want to specify additional
# locations to be searched by find_package and include.
# For example, a local cmake/ direcory within the project, etc.
# list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Display some useful information
cmake_print_variables(CMAKE_MODULE_PATH)
cmake_print_variables(CMAKE_PREFIX_PATH)



################################################################################
#
# Targets
#

# Add the src/ directory to the include path of all targets
include_directories("src")

# Library for expression processing
add_subdirectory("src/expression-lib")

# Application
add_subdirectory("src/application")

# Unit tests
find_package(Catch2 REQUIRED)
add_subdirectory("test")
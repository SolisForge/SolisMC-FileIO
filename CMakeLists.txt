cmake_minimum_required(VERSION 3.25)
project(nbt CXX)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(UNIT_TESTS_ENABLED "Enable unit-test compilation" ON )

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()

find_package(solis REQUIRED)
solis_depend(fmt REQUIRED)

# =============================================================================
# Setup library
# =============================================================================
message(STATUS "Generating \"${PROJECT_NAME}\" targets")
solis_namespace(NAMESPACE minecraft)
solis_library("${PROJECT_NAME}_format" DIRECTORY "src/nbt/format" DEPENDS solis::utils fmt::fmt INCLUDES "include/format")
solis_library("${PROJECT_NAME}_io" DIRECTORY "src/nbt/io" DEPENDS nbt_format INCLUDES "include/io")

if (UNIT_TESTS_ENABLED)
  message(STATUS "Generating \"${PROJECT_NAME}\" tests")
  solis_program(parsing_test DIRECTORY "test/parsing" DEPENDS nbt_io solis::cpp_test INCLUDES "include/tests")
endif()

solis_package()
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
solis_library("${PROJECT_NAME}_format" DIRECTORY "src/nbt/format" DEPENDS solis::utils fmt::fmt)
solis_library("${PROJECT_NAME}_io" DIRECTORY "src/nbt/io" DEPENDS nbt_format)


if (UNIT_TESTS_ENABLED)
  message(STATUS "Generating \"${PROJECT_NAME}\" tests")

  file(GLOB_RECURSE parsing_test_src LIST_DIRECTORIES false "test/parsing/**.cpp")
  add_executable(parsing_test ${parsing_test_src})
  target_link_libraries(parsing_test PRIVATE cpp_test nbt_io)
  solis_register(parsing_test EXE)
endif()

solis_package()
cmake_minimum_required(VERSION 3.25)
project(nbt CXX)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()

find_package(solis REQUIRED)
solis_depend(fmt REQUIRED)

# =============================================================================
# Setup library
# =============================================================================
solis_namespace(NAMESPACE minecraft)
solis_library("${PROJECT_NAME}_format" DIRECTORY "src/nbt/format" DEPENDS solis::utils fmt::fmt)
solis_library("${PROJECT_NAME}_io" DIRECTORY "src/nbt/io" DEPENDS solis::utils fmt::fmt nbt_format)

solis_package()
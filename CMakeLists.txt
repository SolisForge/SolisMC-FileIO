cmake_minimum_required(VERSION 3.18)
project(nbt CXX)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()

find_package(solis REQUIRED)

# =============================================================================
# Setup library
# =============================================================================
solis_library(${PROJECT_NAME} DIRECTORY "src" DEPENDS solis::worlds)

solis_package(NAMESPACE minecraft::)
# =============================================================================
# Project: SOLISMC-IO
# 
# SolisMC-IO project provides the libraries to work with Minecraft files.
# 
# Author    Meltwin (github@meltwin.fr)
# Date      25/08/2026 (created 16/06/2026)
# Version   1.0.0
# Copyright Solis Forge | 2025 
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

cmake_minimum_required(VERSION 3.25)
project(solismc_io CXX)

find_package(solis_cmake REQUIRED)
find_package(solis_lib REQUIRED)
find_package(solis_datagen REQUIRED)

set(CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
option(UNIT_TESTS_ENABLED "Enable unit-test compilation" ON )

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++20)
endif()

enable_testing()

include(nbt/cmake_nbt.cmake)

solis_install()
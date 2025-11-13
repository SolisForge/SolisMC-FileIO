# =============================================================================
# Project: SOLISMC_FILEIO
# 
# SolisMC-FileIO project provides the libraries to work with Minecraft files.
# 
# Author    Meltwin (github@meltwin.fr)
# Date      13/11/2025 (created 13/11/2025)
# Version   1.0.0
# Copyright Solis Forge | 2025 
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

cmake_minimum_required(VERSION 3.25)
project(solismc_fileio CXX)

find_package(solis_cmake REQUIRED)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
option(UNIT_TESTS_ENABLED "Enable unit-test compilation" ON )

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()

include(nbt/cmake_nbt.cmake)

solis_install()
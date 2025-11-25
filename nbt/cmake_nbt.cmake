# =============================================================================
# Project: SOLISMC_FILEIO
# 
# This file contains the CMake definitions of NBT-related targets & tests.
# 
# Author    Meltwin (github@meltwin.fr)
# Date      25/11/2025 (created 13/11/2025)
# Version   1.0.0
# Copyright Solis Forge | 2025 
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

add_solis_library( nbt 
    DIRECTORIES "nbt/src"
    PUBLIC_HEADER "nbt/include"
    NAMESPACE solismc
    SHARED
)

if ("${CMAKE_CXX_BYTE_ORDER}" STREQUAL "BIG_ENDIAN")
  target_compile_definitions(nbt PUBLIC _CMAKE_ENDIANNESS=1)
else()
  target_compile_definitions(nbt PUBLIC _CMAKE_ENDIANNESS=0)
endif()

add_solis_executable( test_parse
    DIRECTORIES "nbt/tests/parser"
    DEPENDS nbt solis_external::doctest
)
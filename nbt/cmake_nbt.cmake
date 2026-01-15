# =============================================================================
# Project: SOLISMC_FILEIO
# 
# This file contains the CMake definitions of NBT-related targets & tests.
# 
# Author    Meltwin (github@meltwin.fr)
# Date      15/01/2026 (created 23/12/2025)
# Version   1.0.0
# Copyright Solis Forge | 2025 
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

option(NBT_BIG_ENDIAN "Do the NBT bytes translator should read values as little-endian (BEDROCK) or big-endian (JAVA)" 1)

# =============================================================================
# NBT library
# =============================================================================
add_solis_library( nbt 
    DIRECTORIES "nbt/src"
    PUBLIC_HEADER "nbt/include"
    NAMESPACE solismc
    SHARED
)
target_compile_definitions(nbt PRIVATE NBT_BIG_ENDIAN=1)

# =============================================================================
# Dataset generation
# =============================================================================
set(DATASET_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/dataset")
add_custom_target( nbt_dataset
    python3 ${CMAKE_CURRENT_LIST_DIR}/data/gen_data.py "${CMAKE_CURRENT_LIST_DIR}/data/models" "${DATASET_GEN_DIR}/solismc_dataset/nbt/"
    COMMENT "Building dataset"
)

# =============================================================================
# Tests
# =============================================================================
add_solis_executable( test_parse
    DIRECTORIES "nbt/tests/parser"
    DEPENDS nbt solis_external::doctest
)
target_include_directories(test_parse PRIVATE "${DATASET_GEN_DIR}")
add_dependencies(test_parse nbt_dataset)
add_test(NAME test_nbt_parse COMMAND test_parse)
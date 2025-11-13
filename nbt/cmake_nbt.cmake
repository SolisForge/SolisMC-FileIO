# =============================================================================
# Project: SOLISMC_FILEIO
# 
# This file contains the CMake definitions of NBT-related targets & tests.
# 
# Author    Meltwin (github@meltwin.fr)
# Date      13/11/2025 (created 13/11/2025)
# Version   1.0.0
# Copyright Solis Forge | 2025 
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

add_solis_library( nbt 
    DIRECTORIES "nbt/src"
    INCLUDES "nbt/include"
    SHARED
)
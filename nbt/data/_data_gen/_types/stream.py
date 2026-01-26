# =============================================================================
# Project: SOLISMC-FILEIO
#
# Byte stream container
#
# Author    Meltwin (github@meltwin.fr)
# Date      26/01/2026 (created 26/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

from dataclasses import dataclass, field


# =============================================================================
@dataclass
class ByteStream:
    """
    Class containing a string
    """

    length_: int = 0
    bytes_: bytes = field(default_factory=bytes)
    values_: list[str] = field(default_factory=list)
    values_length_: list[str] = field(default_factory=list)

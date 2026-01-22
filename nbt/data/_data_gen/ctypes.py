# =============================================================================
# Project: SOLISMC-FILEIO
#
# Ctypes enumeration
#
# Author    Meltwin (github@meltwin.fr)
# Date      21/01/2026 (created 21/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================
from collections.abc import Callable
from enum import Enum


# =============================================================================
class CType(Enum):
    """
    Enumeration of supported ctypes for this dataset generator.
    """

    # Integral values
    BYTE = "int8_t"
    SHORT = "int16_t"
    INT = "int32_t"
    LONG = "int64_t"

    # Floating point values
    FLOAT = "float"
    DOUBLE = "double"

    # Others
    STRING = "std::string_view"

    # -------------------------------------------------------------------------
    def get_byte_length(self) -> int:
        """
        Return the byte length of a value of this type
        """
        match self:
            # Integral types
            case self.BYTE:
                return 1
            case self.SHORT:
                return 2
            case self.INT:
                return 4
            case self.LONG:
                return 8
            # Float types
            case self.FLOAT:
                return 4
            case self.DOUBLE:
                return 8
            # Unsupported
            case _:
                raise TypeError(f"Type {self.value} has no fixed value length")


# =============================================================================
class UnsupporterTypeError(Exception):
    """
    The given type is not supported
    """

    def __init__(self, t: type | str, func: Callable) -> None:
        super().__init__(
            f"Type {t.__class__.__name__ if isinstance(t, type) else t} is unsupported "
            f"for operation {func.__name__}"
        )

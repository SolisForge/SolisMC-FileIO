# =============================================================================
# Project: SOLISMC-FILEIO
#
# Type mapping for the data generator
#
# Author    Meltwin (github@meltwin.fr)
# Date      21/01/2026 (created 21/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================
from .ctypes import CType, UnsupporterTypeError
import struct


# =============================================================================
# Mapping functions
# =============================================================================
def default_value(ctype: CType) -> str:
    """
    Get the default value for the type
    """
    match ctype:
        case CType.BYTE | CType.SHORT | CType.INT | CType.LONG:
            return "0"
        case CType.FLOAT | CType.DOUBLE:
            return "0.0"
        case CType.STRING:
            return '""'

        # Unsupported type
        case _:
            raise UnsupporterTypeError(ctype.value, default_value)


# =============================================================================
def fmt_value(value, ctype: CType) -> str:
    """
    Format the value for CPP.
    """
    match ctype:
        case CType.STRING:
            return f'"{value}"'
        case _:
            return str(value)


# =============================================================================
def to_byte(value, ctype: CType) -> bytes:
    """
    Convert the value to bytes according to the given ctype
    """
    match ctype:
        case CType.SHORT | CType.INT | CType.LONG:
            assert isinstance(value, int)
            return value.to_bytes(ctype.get_byte_length(), "big", signed=True)
        case CType.FLOAT:
            assert isinstance(value, float)
            return struct.pack(">f", value)
        case CType.DOUBLE:
            assert isinstance(value, float)
            return struct.pack(">d", value)

        case CType.STRING:
            assert isinstance(value, str)
            return value.encode()

        # Unsupported type
        case _:
            raise UnsupporterTypeError(ctype.value, to_byte)


# =============================================================================
def to_py_type(ctype: CType) -> type:
    """
    Get the py type from the given ctype
    """
    match ctype:
        case CType.BYTE | CType.SHORT | CType.INT | CType.LONG:
            return int
        case CType.FLOAT | CType.DOUBLE:
            return float
        case CType.STRING:
            return str
        case _:
            return dict

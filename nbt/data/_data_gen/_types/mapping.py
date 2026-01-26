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
from .ctypes import _CType, CType, UnsupporterTypeError
import struct


# =============================================================================
# Mapping functions
# =============================================================================
def default_value(ctype: CType) -> str:
    """
    Get the default value for the type
    """
    match ctype.spec_:
        case _CType.BYTE | _CType.SHORT | _CType.INT | _CType.LONG:
            return "0"
        case _CType.FLOAT | _CType.DOUBLE:
            return "0.0"
        case _CType.STRING:
            return '""'
        case _CType.LIST:
            return "nullptr"

        # Unsupported type
        case _:
            raise UnsupporterTypeError(ctype.value, default_value)


# =============================================================================
def fmt_value(value, ctype: CType) -> str:
    """
    Format the value for CPP.
    """
    match ctype.spec_:
        case _CType.STRING:
            return f'"{value}"'
        case _CType.LIST:
            return ",".join([fmt_value(v, ctype.list_spec) for v in value])
        case _:
            return str(value)


# =============================================================================
def fmt_value_for_stream(value_name: str, ctype: CType) -> str:
    """
    Format the value for CPP.
    """
    match ctype.spec_:
        case _CType.LIST:
            return value_name
        case _:
            return value_name


# =============================================================================
def fmt_type(name: str, ctype: CType, n_values: int) -> str:
    """
    Format the type for CPP
    """
    match ctype.spec_:
        case _CType.LIST:
            return f"{ctype._list_spec_.value} {name}[{n_values}]"
        case _:
            return f"{ctype.spec_.value} {name}"


# =============================================================================
def fmt_type_for_stream(ctype: CType) -> str:
    """
    Format the type for CPP
    """
    match ctype.spec_:
        case _CType.LIST:
            return f"const {ctype._list_spec_.value}*"
        case _:
            return ctype.spec_.value


# =============================================================================
def to_byte(value, ctype: CType) -> bytes:
    """
    Convert the value to bytes according to the given ctype
    """
    match ctype.spec_:
        case _CType.SHORT | _CType.INT | _CType.LONG:
            assert isinstance(value, int)
            return value.to_bytes(ctype.spec_.get_byte_length(), "big", signed=True)
        case _CType.FLOAT:
            assert isinstance(value, float)
            return struct.pack(">f", value)
        case _CType.DOUBLE:
            assert isinstance(value, float)
            return struct.pack(">d", value)

        case _CType.STRING:
            assert isinstance(value, str)
            return value.encode()

        # Unsupported type
        case _:
            raise UnsupporterTypeError(ctype.spec_.value, to_byte)


# =============================================================================
def to_py_type(ctype: CType) -> type:
    """
    Get the py type from the given ctype
    """
    match ctype.spec_:
        case _CType.BYTE | _CType.SHORT | _CType.INT | _CType.LONG:
            return int
        case _CType.FLOAT | _CType.DOUBLE:
            return float
        case _CType.STRING:
            return str
        case _CType.LIST:
            return list[to_py_type(ctype.list_spec)]
        case _:
            return dict

# =============================================================================
# Project: SOLISMC-FILEIO
#
# Global variables for this script
#
# Author    Meltwin (github@meltwin.fr)
# Date      26/01/2026 (created 26/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

from .ctypes import CType
from dataclasses import dataclass
from typing import Any


# =============================================================================
@dataclass
class ValueRegister:

    name: str
    ctype: CType
    value: Any
    stream: bytes


LOADED: dict[str, ValueRegister] = {}
""" Loaded values mapping """


# =============================================================================
def register_value(name: str, ctype: CType, value: Any, stream: bytes) -> None:
    """ "
    Register this value
    """
    LOADED[name] = ValueRegister(name, ctype, value, stream)


# =============================================================================
def get_value(name: str) -> ValueRegister:
    if not is_value_loaded(name):
        raise RuntimeError(f"Trying to get value {name} but it was not yet loaded")
    return LOADED[name]


# =============================================================================
def is_value_loaded(name: str) -> bool:

    return name in LOADED.keys()

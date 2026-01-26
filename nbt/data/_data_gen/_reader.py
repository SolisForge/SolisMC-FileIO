# =============================================================================
# Project: SOLISMC-FILEIO
#
# Dataset model file reading & parsing methods
#
# Author    Meltwin (github@meltwin.fr)
# Date      13/01/2026 (created 13/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================

from dataclasses import dataclass, field
from logging import getLogger
from pathlib import Path
from solis.utils.types.patterns import LazyInit
from ._types import CType, is_value_loaded
from typing import Any, Generic, TypeVar
from ._types.mapping import to_py_type
import yaml

_T = TypeVar("_T")
LOGGER = LazyInit(lambda: getLogger("DataLoad"))


# =============================================================================
@dataclass
class StreamConfig:
    """
    Configuration of a stream
    """

    incomplete: int = 0
    values: list[str] = field(default_factory=list)


# =============================================================================
@dataclass
class DatasetConfig(Generic[_T]):
    """
    Loaded dataset configuration file
    """

    cpp_type_: CType
    """ The c++ type of the value """
    py_type: type[_T]
    """ The python equivalent of the type """
    values: dict[str, _T] = field(default_factory=dict)
    """ Declared values of this type """
    streams: dict[str, StreamConfig] = field(default_factory=dict)
    """ Streams configurations as pair (name of the stream, list of value names) """
    includes: list[str] = field(default_factory=list)
    """ Additional includes for this header """


# =============================================================================
def read_from_file(file_in: Path) -> DatasetConfig:
    """
    Read and parse a dataset configuration from file
    """
    with file_in.open("r") as handle:
        content: dict[str, Any] = yaml.safe_load(handle)  # type: ignore

        # Get values from the config
        ctype = CType.from_str(str(content["ctype"]))
        pytype = to_py_type(ctype)

        config = DatasetConfig(ctype, pytype)

        # Read additional includes
        config.includes = content.get("include", [])

        # Read values
        config.values = (
            {}
            if (values := content["values"]) is None
            else _normalize_values(pytype, values)
        )

        # Read streams
        _parse_stream_content(
            {} if (streams := content["streams"]) is None else streams, config
        )

        return config


# =============================================================================
def _normalize_values(t: type[_T], yml_data: dict[str, Any]) -> dict[str, _T]:
    """Normalize the parsed value to the given type"""
    return {key: t(val) for key, val in yml_data.items()}  # type: ignore


# =============================================================================
def _parse_stream_content(stream: dict[str, dict], config: DatasetConfig) -> None:
    """
    Check the content of the streams for name mismatch.

    Filter out any invalid stream.
    """
    for stream_name, stream_content in stream.items():
        strm = StreamConfig()

        # Read stream flags
        if "incomplete" in stream_content.keys():
            strm.incomplete = int(stream_content["incomplete"])

        # Check values names
        for value in stream_content["values"]:
            if value not in config.values.keys() and not is_value_loaded(value):
                LOGGER().warning(
                    f"Stream {stream_name} requires non-existing value {value}"
                )
                break
        else:
            strm.values = stream_content["values"]
        config.streams[stream_name] = strm

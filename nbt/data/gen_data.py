# =============================================================================
# Project: SOLISMC-FILEIO
#
# Dataset generator for the solismc-nbt library
#
# Author    Meltwin (github@meltwin.fr)
# Date      13/01/2026 (created 13/01/2026)
# Version   1.0.0
# Copyright Solis Forge | 2026
#           Distributed under MIT License (https://opensource.org/licenses/MIT)
# =============================================================================
from argparse import ArgumentParser
from logging import getLogger, config
from pathlib import Path
from solis.resources import LOGGER_CONF
from solis.utils.types.patterns import LazyInit

LOGGER = LazyInit(lambda: getLogger("data_gen"))


# =============================================================================
def process_file(file_in: Path, file_out: Path) -> None:
    """
    Process the given file into an header one
    """
    LOGGER().info(f"Processing file {file_in} -> {file_out}")


# =============================================================================
def gen_data(input_d: Path, output_d: Path) -> None:
    """
    Generate the data described in the YAML files in the input
    """
    LOGGER().info(f"Building dataset from {input_d} in {output_d}")

    # Get file list in order
    files = list(input_d.glob("*.yml"))
    files.sort(key=lambda f: int(f.name.split("_")[0]))

    for file in files:
        process_file(file, output_d.joinpath(file.stem + ".hpp"))


# =============================================================================
# =============================================================================
# =============================================================================
if __name__ == "__main__":
    config.fileConfig(LOGGER_CONF)
    parser = ArgumentParser()
    parser.add_argument(
        "input_folder",
        type=Path,
        help="Folder containing the description of the resources",
    )
    parser.add_argument(
        "output_folder", type=Path, help="Folder where to place generated headere files"
    )
    args = parser.parse_args()

    gen_data(args.input_folder, args.output_folder)

#include "minecraft/nbt/io/nbt_reader.hpp"
#include <filesystem>
#include <fstream>
#include <solis/utils/errors.hpp>
#include <zlib.h>

namespace minecraft::nbt {

NBTReader::NBTReader() {}

// ============================================================================
//    IO related methods
// ============================================================================

std::string NBTReader::get_file_content(const char *filename) {
  // Test for file existence
  if (!std::filesystem::exists(filename))
    throw solis::FileNotFoundError(filename);

  // Fetch file contents
  std::stringstream out;
  std::ifstream handle(filename);
  char buffer[CHUNK_SIZE];
  while (!handle.eof()) {
    handle.read(buffer, CHUNK_SIZE);
    out << buffer;
  }
  handle.close();
  return out.str();
}

} // namespace minecraft::nbt
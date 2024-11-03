#include "binaryio.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace common {

  int BinaryIO::write_binary(std::vector<uint8_t> data, std::string const & output) {
    std::ofstream file(output, std::ios::binary);
    if (!file) { throw std::runtime_error("Error opening file: " + output); }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file.write(reinterpret_cast<char const *>(data.data()),
               static_cast<std::streamsize>(data.size()));
    if (!file) { throw std::runtime_error("Error writing to file: " + output); }

    return 0;
  }

  std::vector<uint8_t> BinaryIO::read_binary(std::string const & input) {
    std::ifstream file(input, std::ios::binary);
    if (!file) { throw std::runtime_error("Error opening file: " + input); }
    file.seekg(0, std::ios::end);
    long const size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> contents(static_cast<size_t>(size));

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file.read(reinterpret_cast<char *>(contents.data()), size);
    if (file.fail()) { throw std::runtime_error("Error reading from file: " + input); }

    return contents;
  }
}  // namespace common
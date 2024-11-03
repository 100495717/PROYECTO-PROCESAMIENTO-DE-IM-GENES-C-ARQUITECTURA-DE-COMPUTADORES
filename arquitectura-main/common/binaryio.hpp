#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace common {
  class BinaryIO {
    public:
      static int write_binary(std::vector<uint8_t> data, std::string const & output);
      static std::vector<uint8_t> read_binary(std::string const & input);
  };
}  // namespace common
#endif
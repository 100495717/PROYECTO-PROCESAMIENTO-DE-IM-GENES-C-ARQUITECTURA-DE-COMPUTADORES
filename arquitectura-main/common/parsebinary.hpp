#ifndef PARSEBINARY_HPP
#define PARSEBINARY_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace common {
  class ParseBinary {
    private:
      static void skip_spaces(std::vector<uint8_t> const & data, size_t & index);
      static void skip_one_space(std::vector<uint8_t> const & data, size_t & index);
      static std::string read_string(std::vector<uint8_t> const & data, size_t & index);
      static std::string read_string_to_end(std::vector<uint8_t> const & data, size_t & index);

    public:
      static std::vector<std::string> parse(std::vector<uint8_t> const & data);
  };
}  // namespace common

#endif
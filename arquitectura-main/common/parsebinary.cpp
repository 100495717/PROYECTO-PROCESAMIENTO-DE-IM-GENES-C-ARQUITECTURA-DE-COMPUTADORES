#include "parsebinary.hpp"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace common {

  void ParseBinary::skip_spaces(std::vector<uint8_t> const & data, size_t & index) {
    skip_one_space(data, index);
    while (std::isspace(data.at(index)) != 0) {
      if (index < data.size() - 1) {
        index++;
      } else {
        throw std::runtime_error("Error en los espacios");
      }
    }
  }

  void ParseBinary::skip_one_space(std::vector<uint8_t> const & data, size_t & index) {
    if (std::isspace(data.at(index)) != 0 && index < data.size() - 1) {
      index++;
    } else {
      throw std::runtime_error("Error en los espacios");
    }
  }

  std::string ParseBinary::read_string(std::vector<uint8_t> const & data, size_t & index) {
    std::string str;
    while (index < data.size() && std::isspace(data.at(index)) == 0) {
      str += static_cast<char>(data.at(index));
      index++;
    }
    if (index >= data.size()) { throw std::runtime_error("Error en la lectura de la cadena"); }
    return str;
  }

  std::string ParseBinary::read_string_to_end(std::vector<uint8_t> const & data, size_t & index) {
    std::string str;
    while (index < data.size()) {
      str += static_cast<char>(data.at(index));
      index++;
    }
    return str;
  }

  std::vector<std::string> ParseBinary::parse(std::vector<uint8_t> const & data) {
    if (data.empty()) { throw std::runtime_error("Error archivo vacio"); }
    std::vector<std::string> result;
    size_t index = 0;

    if (std::isspace(data.at(index)) != 0) {
      throw std::runtime_error("Error en los espacios al inicio");
    }

    result.push_back(read_string(data, index));

    skip_spaces(data, index);

    result.push_back(read_string(data, index));

    skip_spaces(data, index);

    result.push_back(read_string(data, index));

    skip_spaces(data, index);

    result.push_back(read_string(data, index));

    skip_one_space(data, index);

    result.push_back(read_string_to_end(data, index));

    return result;
  }

}  // namespace common
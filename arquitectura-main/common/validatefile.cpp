#include "validatefile.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>

namespace common {
  constexpr int MAX_MAXLEVEL = 65536;

  void validateFile::validate_magic_number(std::string const & string) {
    if (string != "P6") { throw std::invalid_argument("Invalid magic number"); }
  }

  int validateFile::validate_width(std::string const & string) {
    try {
      int const width = std::stoi(string);
      if (width <= 0) { throw std::invalid_argument("Invalid width"); }
      return width;
    } catch (std::invalid_argument const & ia) { throw std::invalid_argument("Invalid width"); }
  }

  int validateFile::validate_height(std::string const & string) {
    try {
      int const height = std::stoi(string);
      if (height <= 0) { throw std::invalid_argument("Invalid height"); }
      return height;
    } catch (std::invalid_argument const & ia) { throw std::invalid_argument("Invalid height"); }
  }

  int validateFile::validate_maxlevel(std::string const & string) {
    try {
      int const maxlevel = std::stoi(string);
      if (maxlevel <= 0 || maxlevel >= MAX_MAXLEVEL) {
        throw std::invalid_argument("Invalid maxlevel");
      }
      return maxlevel;
    } catch (std::invalid_argument const & ia) { throw std::invalid_argument("Invalid maxlevel"); }
  }

  void validateFile::validate_pixels(std::string const & string, size_t number_of_bytes) {
    if (string.size() != number_of_bytes) {
      throw std::invalid_argument("Invalid number of pixels");
    }
  }
}  // namespace common
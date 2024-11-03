#ifndef VALIDATEFILE_HPP
#define VALIDATEFILE_HPP

#include <cstddef>
#include <string>

namespace common {

  class validateFile {
    public:
      static void validate_magic_number(std::string const & string);
      static int validate_width(std::string const & string);
      static int validate_height(std::string const & string);
      static int validate_maxlevel(std::string const & string);
      static void validate_pixels(std::string const & string, size_t number_of_bytes);
  };
}  // namespace common

#endif
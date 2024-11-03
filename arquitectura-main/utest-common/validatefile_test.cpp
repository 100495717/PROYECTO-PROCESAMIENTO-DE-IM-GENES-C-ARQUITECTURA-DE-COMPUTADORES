#include "../common/validatefile.hpp"

#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

// NOLINTBEGIN(cert-err58-cpp)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-goto)
// NOLINTBEGIN(modernize-type-traits)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
namespace common {

  TEST(validate_magic_number, valido) {
    std::string const input = "P6";

    ASSERT_NO_THROW(validateFile::validate_magic_number(input));
  }

  TEST(validate_magic_number, no_valido) {
    std::string const input = "P5";
    try {
      validateFile::validate_magic_number(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid magic number", std::string(ia.what()));
    }
  }

  TEST(validate_maxlevel, valido) {
    std::string const input = "123";
    int maxlevel            = 0;
    ASSERT_NO_THROW(maxlevel = validateFile::validate_maxlevel(input));
    ASSERT_EQ(123, maxlevel);
  }

  TEST(validate_maxlevel, texto) {
    std::string const input = "abc";
    try {
      (void) validateFile::validate_maxlevel(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid maxlevel", std::string(ia.what()));
    }
  }

  TEST(validate_maxlevel, negativo) {
    std::string const input = "-1";
    try {
      (void) validateFile::validate_maxlevel(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid maxlevel", std::string(ia.what()));
    }
  }

  TEST(validate_maxlevel, grande) {
    std::string const input = "123456";
    try {
      (void) validateFile::validate_maxlevel(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid maxlevel", std::string(ia.what()));
    }
  }

  TEST(validate_width, texto) {
    std::string const input = "abc";

    try {
      (void) validateFile::validate_width(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid width", std::string(ia.what()));
    }
  }

  TEST(validate_width, negativo) {
    std::string const input = "-1";

    try {
      (void) validateFile::validate_width(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid width", std::string(ia.what()));
    }
  }

  TEST(validate_height, texto) {
    std::string const input = "abc";

    try {
      (void) validateFile::validate_height(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid height", std::string(ia.what()));
    }
  }

  TEST(validate_height, negativo) {
    std::string const input = "-1";

    try {
      (void) validateFile::validate_height(input);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid height", std::string(ia.what()));
    }
  }

  TEST(validate_pixels, bueno) {
    std::string const input      = "123";
    size_t const number_of_bytes = 3;
    ASSERT_NO_THROW(validateFile::validate_pixels(input, number_of_bytes));
  }

  TEST(validate_pixels, mucho_datos_de_pixels) {
    std::string const input      = "123456789";
    size_t const number_of_bytes = 3;

    try {
      validateFile::validate_pixels(input, number_of_bytes);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid number of pixels", std::string(ia.what()));
    }
  }

  TEST(validate_pixels, pocos_datos_de_pixels) {
    std::string const input      = "123";
    size_t const number_of_bytes = 9;
    try {
      validateFile::validate_pixels(input, number_of_bytes);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid number of pixels", std::string(ia.what()));
    }
  }
}  // namespace common

// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(modernize-type-traits)
// NOLINTEND(cppcoreguidelines-avoid-goto)
// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cert-err58-cpp)
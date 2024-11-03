#include "../imgaos/imageaos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-goto)
// NOLINTBEGIN(modernize-type-traits)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTBEGIN(readability-magic-numbers)
namespace imgaos {
  TEST(resize, invalid_width) {
    std::string const inputs_str = "P6\n2 "
                                   "2\n255\n"
                                   "\x10\x20\x30\x40\x50\x60\x70\x80\x90\xA0\xB0\xC0";

    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    try {
      img.resize(0, 4);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid width", std::string(ia.what()));
    }
  }

  TEST(resize, invalid_height) {
    std::string const inputs_str = "P6\n2 "
                                   "2\n255\n"
                                   "\x10\x20\x30\x40\x50\x60\x70\x80\x90\xA0\xB0\xC0";

    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    try {
      img.resize(4, 0);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & ia) {
      ASSERT_EQ("Invalid height", std::string(ia.what()));
    }
  }

  TEST(resize, uint8_ampliar) {
    std::string const inputs_str = "P6\n2 "
                                   "2\n255\n"
                                   "\x10\x20\x30\x40\x50\x60\x70\x80\x90\xA0\xB0\xC0";

    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    img.resize(4, 4);

    std::vector<uint8_t> const binary = img.toBinary();

    std::string const expected_str =
        "P6\n4 "
        "4\n255\n\x10\x20\x30\x28\x38\x48\x28\x38\x48\x40\x50\x60\x40\x50\x60\x58"
        "\x68\x78\x58\x68\x78\x70\x80\x90\x40\x50\x60\x58\x68\x78\x58\x68\x78\x70"
        "\x80\x90\x70\x80\x90\x88\x98\xa8\x88\x98\xa8\xa0\xb0\xc0";

    std::vector<std::uint8_t> const expected(expected_str.begin(), expected_str.end());

    ASSERT_EQ(expected, binary);
  }

  TEST(resize, uint8_encojer) {
    std::string const inputs_str =
        "P6\n4 "
        "4\n255\n\x10\x20\x30\x28\x38\x48\x28\x38\x48\x40\x50\x60\x40\x50\x60\x58"
        "\x68\x78\x58\x68\x78\x70\x80\x90\x40\x50\x60\x58\x68\x78\x58\x68\x78\x70"
        "\x80\x90\x70\x80\x90\x88\x98\xa8\x88\x98\xa8\xa0\xb0\xc0";
    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    img.resize(2, 2);

    std::vector<uint8_t> const binary = img.toBinary();

    std::string const expected_str = "P6\n2 "
                                     "2\n255\n"
                                     "\x10\x20\x30\x40\x50\x60\x70\x80\x90\xA0\xB0\xC0";

    std::vector<std::uint8_t> const expected(expected_str.begin(), expected_str.end());

    ASSERT_EQ(expected, binary);
  }

  TEST(resize, uint16_encojer) {
    std::string const inputs_str =
        "P6\n3 "
        "3\n65535\n"
        "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12"
        "\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24"
        "\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35"
        "\x36";

    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    img.resize(2, 2);

    std::vector<uint8_t> const binary = img.toBinary();

    std::string const expected_str =
        "P6\n2 "
        "2\n65535\n"
        "\x01\x02\x03\x04\x05\x06\x0D\x0E\x0F\x10\x11\x12\x25\x26\x27\x28\x29\x2A"
        "\x31\x32\x33\x34\x35\x36";

    std::vector<std::uint8_t> const expected(expected_str.begin(), expected_str.end());

    ASSERT_EQ(expected, binary);
  }

  TEST(resize, uint16_ampliar) {
    std::string const inputs_str =

        "P6\n2 2\n65535\n"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30";

    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    img.resize(2, 8);

    std::vector<uint8_t> const binary = img.toBinary();

    std::string const expected_str =
        "P6\n2 "
        "8\n65535\n"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30"
        "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
        "\x30"
        "\x30\x30\x30\x30\x30";

    std::vector<std::uint8_t> const expected(expected_str.begin(), expected_str.end());

    ASSERT_EQ(expected, binary);
  }
}  // namespace imgaos

// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(modernize-type-traits)
// NOLINTEND(cppcoreguidelines-avoid-goto)
// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cert-err58-cpp)
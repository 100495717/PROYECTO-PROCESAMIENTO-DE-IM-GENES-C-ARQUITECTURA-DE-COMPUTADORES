#include "../imgaos/imageaos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <string>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-goto)
// NOLINTBEGIN(modernize-type-traits)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
namespace imgaos {

  TEST(cut_freq, normal) {
    std::string const inputs_str = "P6\n2 "
                                   "8\n255\n"
                                   "\x01\x02\x01\x30\x30\x30\x30\x30\x30\x30\x30"
                                   "\x30\x30\x30\x30\x30\x30\x30\x30"
                                   "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
                                   "\x30\x30\x30\x30\x30\x30\x30\x30"
                                   "\x30\x30\x30\x30\x30\x30\x30\x01\x01\x01";
    std::vector<std::uint8_t> const inputs(inputs_str.begin(), inputs_str.end());

    AOS img(inputs);

    img.cut_freq(1);

    std::vector<uint8_t> const binary = img.toBinary();

    std::string const expected_str = "P6\n2 "
                                     "8\n255\n"
                                     "\x01\x01\x01\x30\x30\x30\x30\x30\x30\x30\x30"
                                     "\x30\x30\x30\x30\x30\x30\x30\x30"
                                     "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30"
                                     "\x30\x30\x30\x30\x30\x30\x30\x30"
                                     "\x30\x30\x30\x30\x30\x30\x30\x01\x01\x01";
    std::vector<std::uint8_t> const expected(expected_str.begin(), expected_str.end());

    ASSERT_EQ(expected, binary);
  }

}  // namespace imgaos

// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(modernize-type-traits)
// NOLINTEND(cppcoreguidelines-avoid-goto)
// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cert-err58-cpp)
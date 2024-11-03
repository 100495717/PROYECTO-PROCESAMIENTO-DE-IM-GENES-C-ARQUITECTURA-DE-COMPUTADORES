#include "../common/binaryio.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-goto)
// NOLINTBEGIN(modernize-type-traits)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
namespace common {

  TEST(write_binary, write) {
    std::vector<uint8_t> const testData = {0x01, 0x02, 0x03, 0x04, 0x05};
    std::string const outputFile        = "test_output.bin";
    BinaryIO::write_binary(testData, outputFile);

    std::ifstream file(outputFile, std::ios::binary);
    std::vector<uint8_t> const fileContents((std::istreambuf_iterator<char>(file)),
                                            std::istreambuf_iterator<char>());
    ASSERT_EQ(testData, fileContents);

    file.close();

    static_cast<void>(std::remove(outputFile.c_str()));
  }

  TEST(write_binary, write_ruta_no_valida) {
    std::vector<uint8_t> const testData = {0x01, 0x02, 0x03};
    std::string const outputFile        = "/test_output.bin";
    try {
      BinaryIO::write_binary(testData, outputFile);
      FAIL() << "Expected std::runtime_error";
    } catch (std::runtime_error const & e) {
      EXPECT_EQ(std::string("Error opening file: ") + outputFile, e.what());
    }
  }

  TEST(read_binary, read) {
    std::vector<uint8_t> const testData = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
    std::string const inputFile         = "test_input.bin";

    std::ofstream file(inputFile, std::ios::binary);
    file.write(reinterpret_cast<char const *>(testData.data()), (std::streamsize) testData.size());
    file.close();

    std::vector<uint8_t> readData;
    readData = BinaryIO::read_binary(inputFile);

    ASSERT_EQ(testData, readData);

    static_cast<void>(std::remove(inputFile.c_str()));
  }

  TEST(read_binary, read_ruta_no_valida) {
    std::string const nonExistentFile = "nonexistent_file.bin";
    try {
      BinaryIO::read_binary(nonExistentFile);
      FAIL() << "Expected std::runtime_error";
    } catch (std::runtime_error const & e) {
      EXPECT_EQ(std::string("Error opening file: ") + nonExistentFile, e.what());
    }
  }
}  // namespace common

// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(modernize-type-traits)
// NOLINTEND(cppcoreguidelines-avoid-goto)
// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cert-err58-cpp)
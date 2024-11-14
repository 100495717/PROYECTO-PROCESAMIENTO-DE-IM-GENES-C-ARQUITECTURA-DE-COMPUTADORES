#include "../common/binaryio.hpp"
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include <string>


TEST(binaryio, read_binary_file) {
    std::ofstream ofs("read_binary_file_test.bin", std::ios::binary);
    std::vector<uint8_t> const data = {0x01, 0x02, 0x03, 0x04};
    ofs.write(reinterpret_cast<char const*>(data.data()), static_cast<std::streamsize>(data.size()));
    ofs.close();

    std::vector<uint8_t> read_data = BinaryIO::readBinaryFile("read_binary_file_test.bin");
    ASSERT_EQ(data, read_data);

}

TEST(binaryio, write_binary_file) {
    std::vector<uint8_t> const data = {0x01, 0x02, 0x03, 0x04};
    BinaryIO::writeBinaryFile("write_binary_file_test.bin", data);

    std::ifstream ifs("write_binary_file_test.bin", std::ios::binary);
    std::vector<uint8_t> read_data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();
    ASSERT_EQ(data, read_data);

   
    
}

TEST(binaryio, read_binary_file_exception) {
    ASSERT_THROW(BinaryIO::readBinaryFile("non_existent_file.bin"), std::runtime_error);
}

TEST(binaryio, write_binary_file_exception) {
    std::vector<uint8_t> const data = {0x01, 0x02, 0x03, 0x04};
    ASSERT_THROW(BinaryIO::writeBinaryFile("/non_existent_folder/write_binary_file_test.bin", data), std::runtime_error);
}
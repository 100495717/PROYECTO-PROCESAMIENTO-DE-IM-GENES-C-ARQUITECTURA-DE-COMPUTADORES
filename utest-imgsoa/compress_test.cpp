#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl> // Incluye toda la biblioteca GSL
#include <gsl/span> // Solo incluye gsl::span
#include <gsl/assert>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>

TEST(compress, compress_normal){
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    img.b = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    std::cout << content << std::endl;
    std::string const expected = "C6 4 4 255 4\n";
    EXPECT_EQ(content.substr(0, expected.size()), expected);    
}

TEST(compress, compress_empty){
    ImageSoa img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.r = {};
    img.g = {};
    img.b = {};

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    std::cout << content << std::endl;

    std::string const expected = "C6 0 0 255 0\n";
    EXPECT_EQ(content.substr(0, expected.size()), expected);

}

TEST(compress, compress_big){
    ImageSoa img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    img.r = std::vector<uint16_t>(1000000, 1);
    img.g = std::vector<uint16_t>(1000000, 1);
    img.b = std::vector<uint16_t>(1000000, 1);

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    std::cout << content << std::endl;

    std::string const expected = "C6 1000 1000 255 1\n";
    EXPECT_EQ(content.substr(0, expected.size()), expected);
}
#include "../imgaos/image-operations-aos.hpp"

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
    ImageAos img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1} };
    img.pixels = pixels;


    std::string output = "compress_test";
    compress_image(img, output);
    std::ifstream file(output + ".cppm", std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    std::string const expected_header = "C6 4 4 255 4\n";
    EXPECT_EQ(content.substr(0, expected_header.size()), expected_header);   

    // Verificar la tabla de colores para (1,0,0), (0,1,0), (0,0,1), (1,1,1)
    std::string const expected_colors = "\x01\x00\x00\x00\x01\x00\x00\x00\x01\x01\x01";
    EXPECT_EQ(content.substr(expected_header.size(), expected_colors.size()), expected_colors);
    file.close();
}

TEST(compress, compress_empty){
    ImageAos img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.pixels = {};

    std::string output = "compress_test";
    compress_image(img, output);
    std::ifstream file(output + ".cppm", std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string const expected = "C6 0 0 255 0\n";
    EXPECT_EQ(content.substr(0, expected.size()), expected);

}

TEST(compress, compress_big){
    ImageAos img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    std::vector<Pixel> pixels(1000000, {1, 1, 1});
    img.pixels = pixels;

    std::string output = "compress_test";
    compress_image(img, output);
    std::ifstream file(output + ".cppm", std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string const expected_header = "C6 1000 1000 255 1\n";
    EXPECT_EQ(content.substr(0, expected_header.size()), expected_header);

    // Verificar la tabla de colores para (1,1,1)
    std::string const expected_colors = "\x01\x01\x01";
    EXPECT_EQ(content.substr(expected_header.size(), expected_colors.size()), expected_colors);

    file.close();
}
#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

TEST(info, info_normal) {
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    img.b = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    print_image_info_soa(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 4\nAlto: 4\nValor máximo de color: 255\n";
    EXPECT_EQ(buffer.str(), expected);
}

TEST(info, info_empty_image) {
    ImageSoa img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.r = {};
    img.g = {};
    img.b = {};

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    print_image_info_soa(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 0\nAlto: 0\nValor máximo de color: 255\n";
    EXPECT_EQ(buffer.str(), expected);
}

TEST(info, info_big_image) {
    ImageSoa img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    img.r = std::vector<uint16_t>(1000000, 1);
    img.g = std::vector<uint16_t>(1000000, 1);
    img.b = std::vector<uint16_t>(1000000, 1);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    print_image_info_soa(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 1000\nAlto: 1000\nValor máximo de color: 255\n";
    EXPECT_EQ(buffer.str(), expected);
}
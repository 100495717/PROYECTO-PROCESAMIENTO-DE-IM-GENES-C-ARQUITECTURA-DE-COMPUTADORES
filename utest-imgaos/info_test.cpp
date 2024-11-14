#include "../imgaos/image-operations-aos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

TEST(info, info_normal) {
    ImageAos img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1} };
    img.pixels = pixels;

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    print_image_info(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 4\nAlto: 4\nValor máximo de color: 255\nPíxeles totales: 16\n";
    EXPECT_EQ(buffer.str(), expected);
}

TEST(info, info_empty_image) {
    ImageAos img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.pixels = {};

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    print_image_info(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 0\nAlto: 0\nValor máximo de color: 255\nPíxeles totales: 0\n";
    EXPECT_EQ(buffer.str(), expected);
}

TEST(info, info_big_image) {
    ImageAos img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    std::vector<Pixel> pixels(1000000, {1, 1, 1});
    img.pixels = pixels;

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    print_image_info(img);

    std::cout.rdbuf(old);
    std::string const expected = "Ancho: 1000\nAlto: 1000\nValor máximo de color: 255\nPíxeles totales: 1000000\n";
    EXPECT_EQ(buffer.str(), expected);
}
#include "../imgaos/image-operations-aos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl> // Incluye toda la biblioteca GSL
#include <gsl/span> // Solo incluye gsl::span
#include <gsl/assert>
#include <stdexcept>
#include <vector>
#include <string>

TEST(resize, resizetosmaller) {
    ImageAos img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                                  {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1} };
    img.pixels = pixels;


    resize_image(img, 2, 2);

    int const expected_width = 2;
    int const expected_height = 2;
    int const expected_max_color_value = 255;
    std::vector<Pixel> const expected_pixels = { {1,0,0}, {0,0,1},
                                                 {1,0,0}, {0,0,1} };
    EXPECT_EQ(img.width, expected_width);
    EXPECT_EQ(img.height, expected_height);
    EXPECT_EQ(img.max_color_value, expected_max_color_value);
    EXPECT_EQ(img.pixels, expected_pixels);

    

}


TEST(resize, resizetobigger){
    ImageAos img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0},
                                  {1,0,0}, {0,1,0} };
    img.pixels = pixels;

    resize_image(img, 4, 4);

    int const expected_width = 4;
    int const expected_height = 4;
    int const expected_max_color_value = 255;
    std::vector<Pixel> const expected_pixels = { {1,0,0}, {1,0,0}, {0,1,0}, {0,1,0},
                                                 {1,0,0}, {1,0,0}, {0,1,0}, {0,1,0},
                                                 {1,0,0}, {1,0,0}, {0,1,0}, {0,1,0},
                                                 {1,0,0}, {1,0,0}, {0,1,0}, {0,1,0} };
    EXPECT_EQ(img.width, expected_width);
    EXPECT_EQ(img.height, expected_height);
    EXPECT_EQ(img.max_color_value, expected_max_color_value);
    EXPECT_EQ(img.pixels, expected_pixels);
}


TEST(resize, invalid_width){
    ImageAos img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0},
                                  {1,0,0}, {0,1,0} };
    img.pixels = pixels;

    EXPECT_THROW(resize_image(img, 0, 2), std::invalid_argument);
    EXPECT_THROW(resize_image(img, -1, 2), std::invalid_argument);
}

TEST(resize, invalid_height){
    ImageAos img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    std::vector<Pixel> pixels = { {1,0,0}, {0,1,0},
                                  {1,0,0}, {0,1,0} };
    img.pixels = pixels;

    EXPECT_THROW(resize_image(img, 2, 0), std::invalid_argument);
    EXPECT_THROW(resize_image(img, 2, -1), std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
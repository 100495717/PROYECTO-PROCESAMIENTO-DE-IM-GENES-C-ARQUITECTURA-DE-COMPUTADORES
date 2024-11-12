#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>

TEST(resize, resizetosmaller) {
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    img.b = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    resize_image_soa(img, 2, 2);

    int const expected_width = 2;
    int const expected_height = 2;
    int const expected_max_color_value = 255;
    std::vector<uint16_t> const expected_r = {1, 0, 1, 0};
    std::vector<uint16_t> const expected_g = {0, 1, 0, 1};
    std::vector<uint16_t> const expected_b = {0, 1, 0, 1};

    EXPECT_EQ(img.width, expected_width);
    EXPECT_EQ(img.height, expected_height);
    EXPECT_EQ(img.max_color_value, expected_max_color_value);
    EXPECT_EQ(img.r, expected_r);
    EXPECT_EQ(img.g, expected_g);
    EXPECT_EQ(img.b, expected_b);
}

TEST(resize, resizetobigger) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.r = {1, 0, 1, 0};
    img.g = {0, 1, 0, 1};
    img.b = {0, 0, 1, 1};

    resize_image_soa(img, 4, 4);

    int const expected_width = 4;
    int const expected_height = 4;
    int const expected_max_color_value = 255;
    std::vector<uint16_t> const expected_r = {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
    std::vector<uint16_t> const expected_g = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
    std::vector<uint16_t> const expected_b = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};

    EXPECT_EQ(img.width, expected_width);
    EXPECT_EQ(img.height, expected_height);
    EXPECT_EQ(img.max_color_value, expected_max_color_value);
    EXPECT_EQ(img.r, expected_r);
    EXPECT_EQ(img.g, expected_g);
    EXPECT_EQ(img.b, expected_b);
}

TEST(resize, resizetobigger65535) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 65535;
    img.r = {1, 0, 1, 0};
    img.g = {0, 1, 0, 1};
    img.b = {0, 0, 1, 1};

    resize_image_soa(img, 4, 4);

    int const expected_width = 4;
    int const expected_height = 4;
    int const expected_max_color_value = 65535;
    std::vector<uint16_t> const expected_r = {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
    std::vector<uint16_t> const expected_g = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
    std::vector<uint16_t> const expected_b = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};

    EXPECT_EQ(img.width, expected_width);
    EXPECT_EQ(img.height, expected_height);
    EXPECT_EQ(img.max_color_value, expected_max_color_value);
    EXPECT_EQ(img.r, expected_r);
    EXPECT_EQ(img.g, expected_g);
    EXPECT_EQ(img.b, expected_b);
}

TEST(resize, invalid_width) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.r = {1, 0, 1, 0};
    img.g = {0, 1, 0, 1};
    img.b = {0, 0, 1, 1};

    EXPECT_THROW(resize_image_soa(img, 0, 2), std::invalid_argument);
    EXPECT_THROW(resize_image_soa(img, -1, 2), std::invalid_argument);
}

TEST(resize, invalid_height) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.r = {1, 0, 1, 0};
    img.g = {0, 1, 0, 1};
    img.b = {0, 0, 1, 1};

    EXPECT_THROW(resize_image_soa(img, 2, 0), std::invalid_argument);
    EXPECT_THROW(resize_image_soa(img, 2, -1), std::invalid_argument);
}
#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>

TEST(maxlevel, max_level_normal) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.r = {100, 50, 25, 0};
    img.g = {150, 75, 50, 25};
    img.b = {200, 100, 75, 50};

    std::cout << "Píxeles antes de aplicar max_level:" << std::endl;
    for (size_t i = 0; i < img.r.size(); ++i) {
        std::cout << "(" << img.r[i] << ", " << img.g[i] << ", " << img.b[i] << ") ";
    }
    std::cout << std::endl;

    max_level(img, 100);

    std::cout << "Píxeles después de aplicar max_level:" << std::endl;
    for (size_t i = 0; i < img.r.size(); ++i) {
        std::cout << "(" << img.r[i] << ", " << img.g[i] << ", " << img.b[i] << ") ";
    }
    std::cout << std::endl;

    // Verificar que los valores de los píxeles se han escalado correctamente
    ASSERT_EQ(img.r[0], 39);
    ASSERT_EQ(img.g[0], 59);
    ASSERT_EQ(img.b[0], 78);
    ASSERT_EQ(img.r[1], 20);
    ASSERT_EQ(img.g[1], 29);
    ASSERT_EQ(img.b[1], 39);
    ASSERT_EQ(img.r[2], 10);
    ASSERT_EQ(img.g[2], 20);
    ASSERT_EQ(img.b[2], 29);
    ASSERT_EQ(img.r[3], 0);
    ASSERT_EQ(img.g[3], 10);
    ASSERT_EQ(img.b[3], 20);
}

TEST(maxlevel, max_level_invalid_maxlevel) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.r = {100, 50, 25, 0};
    img.g = {150, 75, 50, 25};
    img.b = {200, 100, 75, 50};

    EXPECT_THROW(max_level(img, 0), std::invalid_argument);
    EXPECT_THROW(max_level(img, 70000), std::invalid_argument);
}

TEST(maxlevel, max_level_empty_image) {
    ImageSoa img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.r = {};
    img.g = {};
    img.b = {};

    max_level(img, 100);

    // Verificar que la imagen sigue vacía
    ASSERT_TRUE(img.r.empty());
    ASSERT_TRUE(img.g.empty());
    ASSERT_TRUE(img.b.empty());
}
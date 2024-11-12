#include "../imgaos/image-operations-aos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl> // Incluye toda la biblioteca GSL
#include <gsl/span> // Solo incluye gsl::span
#include <gsl/assert>
#include <stdexcept>
#include <vector>
#include <string>

TEST(maxlevel, max_level_normal) {
    ImageAos img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.pixels = { {100, 150, 200}, {50, 75, 100}, {25, 50, 75}, {0, 25, 50} };

    std::cout << "Píxeles antes de aplicar max_level:" << std::endl;
    print_pixels(img.pixels);

    max_level(img, 100);

    std::cout << "Píxeles después de aplicar max_level:" << std::endl;
    print_pixels(img.pixels);

    // Verificar que los valores de los píxeles se han escalado correctamente
    ASSERT_EQ(img.pixels[0], (Pixel{39, 59, 78}));
    ASSERT_EQ(img.pixels[1], (Pixel{20, 29, 39}));
    ASSERT_EQ(img.pixels[2], (Pixel{10, 20, 29}));
    ASSERT_EQ(img.pixels[3], (Pixel{0, 10, 20}));
}
TEST(maxlevel, max_level_invalid_maxlevel) {
    ImageAos img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 255;
    img.pixels = { {100, 150, 200}, {50, 75, 100}, {25, 50, 75}, {0, 25, 50} };

    EXPECT_THROW(max_level(img, 0), std::invalid_argument);
    EXPECT_THROW(max_level(img, 70000), std::invalid_argument);
}

TEST(maxlevel, max_level_empty_image) {
    ImageAos img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.pixels = {};

    max_level(img, 100);

    // Verificar que la imagen sigue vacía
    ASSERT_TRUE(img.pixels.empty());
}

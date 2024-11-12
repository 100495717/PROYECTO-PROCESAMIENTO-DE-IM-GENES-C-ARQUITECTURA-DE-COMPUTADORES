#include "../imgaos/image-operations-aos.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl> // Incluye toda la biblioteca GSL
#include <gsl/span> // Solo incluye gsl::span
#include <gsl/assert>
#include <stdexcept>
#include <vector>
#include <string>
#include <unordered_map>

TEST(cutfreq, cut_freq_normal) {
    ImageAos img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.pixels = { {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1} };

    cut_freq(img, 2);

    // Verificar que los colores menos frecuentes han sido reemplazados
    std::unordered_map<Pixel, int> freq_map;
    for (const auto& pixel : img.pixels) {
        freq_map[pixel]++;
        std::cout << "Pixel(" << pixel.r << ", " << pixel.g << ", " << pixel.b << "): " << freq_map[pixel] << std::endl;
    }

    // Verificar que los colores menos frecuentes han sido reemplazados
    ASSERT_EQ(freq_map.size(), 2); // Solo deben quedar 2 colores únicos
}
TEST(cutfreq, cut_freq_invalid_n) {
    ImageAos img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.pixels = { {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1},
                   {1,0,0}, {0,1,0}, {0,0,1}, {1,1,1} };

    EXPECT_THROW(cut_freq(img, 0), std::invalid_argument);
    EXPECT_THROW(cut_freq(img, 10), std::invalid_argument);
}

TEST(cutfreq, cut_freq_empty_image) {
    ImageAos img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.pixels = {};

    EXPECT_THROW(cut_freq(img, 1), std::invalid_argument);
}

TEST(cutfreq, cutfreq_big_image) {
    ImageAos img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    std::vector<Pixel> pixels(50000, {1, 1, 1});
    pixels.insert(pixels.end(), 50000, {1, 0, 0});
    pixels.insert(pixels.end(), 50000, {0, 0, 1});
    img.pixels = pixels;

    cut_freq(img,1);

    // Verificar que los colores menos frecuentes han sido reemplazados
    std::unordered_map<Pixel, int> freq_map;
    for (const auto& pixel : img.pixels) {
        freq_map[pixel]++;
        std::cout << "Pixel(" << pixel.r << ", " << pixel.g << ", " << pixel.b << "): " << freq_map[pixel] << std::endl;
    }

    // Verificar que los colores menos frecuentes han sido reemplazados
    ASSERT_EQ(freq_map.size(), 2); // Solo debe quedar 1 color único
}
#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <unordered_map>

// Definir un array para representar el color
using Color = std::array<uint16_t, 3>;

// Definir un hash personalizado para Color
struct ColorHash {
    std::size_t operator()(const Color& color) const {
        auto h1 = std::hash<uint16_t>{}(color[0]);
        auto h2 = std::hash<uint16_t>{}(color[1]);
        auto h3 = std::hash<uint16_t>{}(color[2]);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

TEST(cutfreq_test, cutfreqnormal){
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.b = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};

    cutfreq(img, 2);

    // Verificar que los colores menos frecuentes han sido reemplazados
    std::unordered_map<Color, int, ColorHash> freq_map;
    for (size_t i = 0; i < img.r.size(); ++i) {
        Color pixel = {img.r[i], img.g[i], img.b[i]};
        freq_map[pixel]++;
        std::cout << "Pixel(" << img.r[i] << ", " << img.g[i] << ", " << img.b[i] << "): " << freq_map[pixel] << std::endl;
    }

    // Verificar que los colores menos frecuentes han sido reemplazados
    ASSERT_EQ(freq_map.size(), 2); // Solo deben quedar 2 colores únicos
}

TEST(cutfreq_test, cut_freq_invalid_n) {
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    img.b = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    EXPECT_THROW(cutfreq(img, 0), std::invalid_argument);
    EXPECT_THROW(cutfreq(img, 10), std::invalid_argument);
}

TEST(cutfreq_test, cut_freq_empty_image) {
    ImageSoa img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.r = {};
    img.g = {};
    img.b = {};

    EXPECT_THROW(cutfreq(img, 1), std::invalid_argument);
}

TEST(cutfreq_test, cutfreq_big_image) {
    ImageSoa img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    img.r = std::vector<uint16_t>(1000000, 1);
    img.g = std::vector<uint16_t>(1000000, 1);
    img.b = std::vector<uint16_t>(1000000, 1);

    cutfreq(img, 1);

    // Verificar que los colores menos frecuentes han sido reemplazados
    std::unordered_map<Color, int, ColorHash> freq_map;
    for (size_t i = 0; i < img.r.size(); ++i) {
        Color pixel = {img.r[i], img.g[i], img.b[i]};
        freq_map[pixel]++;
        std::cout << "Pixel(" << img.r[i] << ", " << img.g[i] << ", " << img.b[i] << "): " << freq_map[pixel] << std::endl;
    }

    // Verificar que los colores menos frecuentes han sido reemplazados
    ASSERT_EQ(freq_map.size(), 1); // Solo debe quedar 1 color único
}
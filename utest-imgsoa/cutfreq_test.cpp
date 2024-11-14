#include "../imgsoa/image-operations-soa.hpp"
#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <unordered_map>

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
    }

    // Verificar que los colores menos frecuentes han sido reemplazados
    ASSERT_EQ(freq_map.size(), 1); // Solo debe quedar 1 color único
}

// PROBAR CON CASOS LIMITES {0, 0, 0}, {255, 255, 255}
TEST(cutfreq_test, uniform_color_image) {
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    img.g = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    img.b = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

    cutfreq(img, 1);

    // Asegurarse de que todos los píxeles se mantengan sin cambio
    for (size_t i = 0; i < img.r.size(); ++i) {
        ASSERT_EQ(img.r[i], 100);
        ASSERT_EQ(img.g[i], 100);
        ASSERT_EQ(img.b[i], 100);
    }
}


// Test para verificar el correcto funcionamiento de la función nearestNeighbor
TEST(kdtree_test, nearest_neighbor_simple_case) {
    std::vector<Color> colors = {{100, 100, 100}, {150, 150, 150}, {200, 200, 200}};
    KDTree tree(colors);
    Color target = {150, 150, 150};
    Color result = tree.nearestNeighbor(target);

    ASSERT_EQ(result[0], 150);
    ASSERT_EQ(result[1], 150);
    ASSERT_EQ(result[2], 150);
}


// Test para verificar que solo los colores menos frecuentes se sustituyen por los más cercanos
TEST(cutfreq_test, diverse_colors_image) {
    ImageSoa img;
    img.width = 3;
    img.height = 3;
    img.max_color_value = 255;
    img.r = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    img.g = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    img.b = {1, 2, 3, 1, 2, 3, 1, 2, 3};

    cutfreq(img, 2);

    std::unordered_map<Color, int, ColorHash> freq_map;
    for (size_t i = 0; i < img.r.size(); ++i) {
        Color pixel = {img.r[i], img.g[i], img.b[i]};
        freq_map[pixel]++;
    }

    ASSERT_EQ(freq_map.size(), 1); // Solo debe quedar un color único (los reemplazados por el más cercano)
}


// Test para validar que el algoritmo funciona en un tiempo razonable para imágenes grandes y complejas
TEST(cutfreq_test, performance_large_image) {
    ImageSoa img;
    img.width = 500;
    img.height = 500;
    img.max_color_value = 255;
    for (size_t i = 0; i < 500 * 500; ++i) {
        img.r.push_back(i % 256);
        img.g.push_back((i / 256) % 256);
        img.b.push_back((i / 256) % 256);
    }

    cutfreq(img, 50);

    ASSERT_TRUE(img.r.size() == 500 * 500); // Verificar que todos los píxeles fueron procesados
}

// Test para verificar que los colores menos frecuentes han sido reemplazados correctamente
TEST(cutfreq_test, specific_replacement) {
    ImageSoa img;
    img.width = 3;
    img.height = 3;
    img.max_color_value = 255;
    img.r = {1, 2, 3, 2, 3, 3};
    img.g = {1, 2, 3, 2, 3, 3};
    img.b = {1, 2, 3, 2, 3, 3};

    // Definir los colores esperados después del reemplazo
    std::unordered_map<Color, Color, ColorHash> expected_replacements;
    expected_replacements[{1, 1, 1}] = {3, 3, 3}; // {1, 1, 1} se reemplaza por {3, 3, 3}
    expected_replacements[{2, 2, 2}] = {3, 3, 3}; // {2, 2, 2} se reemplaza por {3, 3, 3}

    cutfreq(img, 2);

    // Verificar que los colores menos frecuentes han sido reemplazados correctamente
    for (size_t i = 0; i < img.r.size(); ++i) {
        Color original_color = {img.r[i], img.g[i], img.b[i]};
        if (expected_replacements.find(original_color) != expected_replacements.end()) {
            Color expected_color = expected_replacements[original_color];
            ASSERT_EQ(img.r[i], expected_color[0]);
            ASSERT_EQ(img.g[i], expected_color[1]);
            ASSERT_EQ(img.b[i], expected_color[2]);
        }
    }
}

// Test para verificar que los colores menos frecuentes son reemplazados por los colores más cercanos en una imagen con colores similares
TEST(cutfreq_test, nearest_neighbor_precision) {
    // Preparar una imagen con colores muy similares
    ImageSoa img;
    img.width = 3;
    img.height = 3;
    img.max_color_value = 255;
    img.r = {100, 101, 102, 100, 101, 102, 102, 100, 101, 102, 103, 103, 103, 103};
    img.g = {100, 101, 102, 100, 101, 102, 102, 100, 101, 102, 103, 103, 103, 103};
    img.b = {100, 101, 102, 100, 101, 102, 102, 100, 101, 102, 103, 103, 103, 103};

    // Definir los colores esperados después del reemplazo
    std::unordered_map<Color, Color, ColorHash> expected_replacements;
    expected_replacements[{100, 100, 100}] = {102, 102, 102}; // {100, 100, 100} se reemplaza por {102, 102, 102}
    expected_replacements[{101, 101, 101}] = {102, 102, 102}; // {101, 101, 101} se reemplaza por {102, 102, 102}

    cutfreq(img, 2);

    // Verificar que los colores menos frecuentes han sido reemplazados correctamente
    for (size_t i = 0; i < img.r.size(); ++i) {
        Color original_color = {img.r[i], img.g[i], img.b[i]};
        if (expected_replacements.find(original_color) != expected_replacements.end()) {
            Color expected_color = expected_replacements[original_color];
            ASSERT_EQ(img.r[i], expected_color[0]);
            ASSERT_EQ(img.g[i], expected_color[1]);
            ASSERT_EQ(img.b[i], expected_color[2]);
        }
    }
}
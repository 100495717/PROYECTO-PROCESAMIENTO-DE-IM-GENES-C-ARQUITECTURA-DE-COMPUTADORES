#include "../imgsoa/image-operations-soa.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <gsl/gsl> // Incluye toda la biblioteca GSL
#include <gsl/span> // Solo incluye gsl::span
#include <gsl/assert>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>

void remove_test_file(const std::string& filename) {
    std::remove(filename.c_str());
}

TEST(compress, compress_normal) {
    ImageSoa img;
    img.width = 4;
    img.height = 4;
    img.max_color_value = 255;
    img.r = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    img.g = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    img.b = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Verificación del encabezado y de la tabla de colores (4 colores únicos)
    std::string const expected_header = "C6 4 4 255 4\n";
    EXPECT_EQ(content.substr(0, expected_header.size()), expected_header);
    
    // Verificar la tabla de colores para (1,0,0), (0,1,0), (0,0,1), (1,1,1)
    std::string const expected_colors = "\x01\x00\x00\x00\x01\x00\x00\x00\x01\x01\x01";
    EXPECT_EQ(content.substr(expected_header.size(), expected_colors.size()), expected_colors);

    file.close();
    remove_test_file(output);
}

TEST(compress, compress_empty) {
    ImageSoa img;
    img.width = 0;
    img.height = 0;
    img.max_color_value = 255;
    img.r = {};
    img.g = {};
    img.b = {};

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string const expected = "C6 0 0 255 0\n"; // No colores
    EXPECT_EQ(content, expected);

    file.close();
    remove_test_file(output);
}

TEST(compress, compress_max_color_value_65535) {
    ImageSoa img;
    img.width = 2;
    img.height = 2;
    img.max_color_value = 65535;
    img.r = {30000, 40000, 50000, 60000};
    img.g = {30000, 40000, 50000, 60000};
    img.b = {30000, 40000, 50000, 60000};

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Verificar encabezado y tabla de colores para valores en 2 bytes por canal
    std::string const expected_header = "C6 2 2 65535 4\n";
    EXPECT_EQ(content.substr(0, expected_header.size()), expected_header);

    // Verificar tabla de colores en little-endian
    std::string const expected_colors = 
        "\x30\x75\x30\x75\x30\x75"  // Color 1: 30000, 30000, 30000
        "\x40\x9C\x40\x9C\x40\x9C"  // Color 2: 40000, 40000, 40000
        "\x50\xC3\x50\xC3\x50\xC3"  // Color 3: 50000, 50000, 50000
        "\x60\xEA\x60\xEA\x60\xEA"; // Color 4: 60000, 60000, 60000
    EXPECT_EQ(content.substr(expected_header.size(), expected_colors.size()), expected_colors);

    file.close();
    remove_test_file(output);
}


TEST(compress, compress_large_image) {
    ImageSoa img;
    img.width = 1000;
    img.height = 1000;
    img.max_color_value = 255;
    img.r = std::vector<uint16_t>(1000000, 1);
    img.g = std::vector<uint16_t>(1000000, 1);
    img.b = std::vector<uint16_t>(1000000, 1);

    std::string output = "compress_test";
    compress_image_soa(img, output);
    std::ifstream file(output, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string const expected_header = "C6 1000 1000 255 1\n";
    EXPECT_EQ(content.substr(0, expected_header.size()), expected_header);

    file.close();
    remove_test_file(output);
}
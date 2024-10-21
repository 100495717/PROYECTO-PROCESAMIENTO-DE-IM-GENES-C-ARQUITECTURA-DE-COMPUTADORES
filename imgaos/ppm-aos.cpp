#include "ppm-aos.hpp"
#include "imageaos.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>


ImageAos read_ppm_image(const std::string& filename) {
    // Abrir archivo binario
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file) {
        throw std::runtime_error("No se pudo abrir el archivo para lectura");
    }

    // Leer todo el archivo en un vector de bytes
    std::vector<uint8_t> file_data((std::istreambuf_iterator<char>(input_file)),
                                   std::istreambuf_iterator<char>());

    // Crear el BinaryReader con el buffer
    BinaryReader reader(file_data);

    // Leer el encabezado PPM
    PPMHeader header = read_ppm_header(reader);

    // Crear una instancia de ImageAos con la información de ancho, alto y max_color_value
    ImageAos image(header.width, header.height, header.maxval);

    // Leer los píxeles de acuerdo al valor máximo de color
    if (header.maxval <= 255) {
        // Leer 3 bytes (RGB) por cada píxel
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                Pixel p;
                p.r = reader.read_byte();
                p.g = reader.read_byte();
                p.b = reader.read_byte();
                image.pixels[i * header.width + j] = p; // Almacenar el píxel en la imagen
            }
        }
    } else {
        // Leer 6 bytes (RGB, 2 bytes por color) por cada píxel en formato little-endian
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                Pixel p;
                p.r = reader.read_word_le();
                p.g = reader.read_word_le();
                p.b = reader.read_word_le();
                image.pixels[i * header.width + j] = p; // Almacenar el píxel en la imagen
            }
        }
    }

    return image;
}


void write_ppm_image(const std::string& filename, const ImageAos& image) {
    // Abrir archivo para escritura en modo binario
    std::ofstream output_file(filename, std::ios::binary);
    if (!output_file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    // Escribir el encabezado PPM
    output_file << "P6\n" << image.width << " " << image.height << "\n" << image.max_color_value << "\n";

    // Escribir los píxeles en formato binario
    if (image.max_color_value <= 255) {
        // Escribir 3 bytes (RGB) por cada píxel
        for (const auto& pixel : image.pixels) {
            output_file.write(reinterpret_cast<const char*>(&pixel.r), 1);
            output_file.write(reinterpret_cast<const char*>(&pixel.g), 1);
            output_file.write(reinterpret_cast<const char*>(&pixel.b), 1);
        }
    } else {
        // Escribir 6 bytes (RGB, 2 bytes por color) por cada píxel en formato little-endian
        for (const auto& pixel : image.pixels) {
            uint16_t r_le = static_cast<uint16_t>(pixel.r);
            uint16_t g_le = static_cast<uint16_t>(pixel.g);
            uint16_t b_le = static_cast<uint16_t>(pixel.b);

            output_file.write(reinterpret_cast<const char*>(&r_le), 2);
            output_file.write(reinterpret_cast<const char*>(&g_le), 2);
            output_file.write(reinterpret_cast<const char*>(&b_le), 2);
        }
    }
}
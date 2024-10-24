#include "ppm-aos.hpp"
#include "imageaos.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>


ImageAos read_ppm(BinaryReader& reader, const PPMHeader& header) {
    // Crear una instancia de ImageAos con la información de ancho, alto y max_color_value
    ImageAos image(header.width, header.height, header.max_color_value);
    // Leer los píxeles de acuerdo al valor máximo de color
    if (header.max_color_value <= 255) {
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

void write_ppm(const std::string& filename, const ImageAos& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "P6\n" << image.width << " " << image.height << "\n" << image.max_color_value << "\n";

    if (image.max_color_value < 256) {
        
        //Cada píxel está representado por 3 bytes
        for (const auto& pixel : image.pixels) {
            unsigned char r = static_cast<unsigned char>(pixel.r);
            unsigned char g = static_cast<unsigned char>(pixel.g);
            unsigned char b = static_cast<unsigned char>(pixel.b);
            file.write(reinterpret_cast<char*>(r), 1);
            file.write(reinterpret_cast<char*>(g), 1);
            file.write(reinterpret_cast<char*>(b), 1);
        }
    } else {
        for (const auto& pixel : image.pixels) {
            unsigned short r = pixel.r;
            unsigned short g = pixel.g;
            unsigned short b = pixel.b;
            file.write(reinterpret_cast<const char*>(&r), 2);
            file.write(reinterpret_cast<const char*>(&g), 2);
            file.write(reinterpret_cast<const char*>(&b), 2);
        }
    }
}


void write_ppm_image(BinaryWriter& writer, const ImageAos& image) {
    for (const Pixel& pixel : image.pixels) {
        // Escribir los tres componentes RGB de cada píxel
        writer.write_byte(pixel.r);
        writer.write_byte(pixel.g);
        writer.write_byte(pixel.b);
    }
}
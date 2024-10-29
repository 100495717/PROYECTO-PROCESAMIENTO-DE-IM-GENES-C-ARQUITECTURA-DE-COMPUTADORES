#include "ppm-aos.hpp"
#include "imageaos.hpp"
#include "common/binaryio.hpp"
#include "common/header_ppm.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>


ImageAos read_ppm_image(const std::string& filename) {

    //Leer el archivo binario
    std::vector<uint8_t> data = read_binary_file(filename);
    BinaryReader reader(data);
    // Leer el encabezado del archivo PPM
    PPMHeader header = read_ppm_header(reader);
    
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

void write_ppm_image(const std::string& filename, const ImageAos& image) {
    // Crear un objeto BinaryWriter con un buffer vacío
    std::vector<uint8_t> buffer;
    BinaryWriter writer(buffer);
    // Escribir el encabezado del archivo PPM
    PPMHeader header;
    header.magic = "P6";
    header.width = image.width;
    header.height = image.height;
    header.max_color_value = image.max_color_value;
    write_ppm_header(writer, header);

    
    if (image.max_color_value < 256) {
        
        //Cada píxel está representado por 3 bytes
        for (const auto& pixel : image.pixels) {
            writer.write_byte(pixel.r);
            writer.write_byte(pixel.g);
            writer.write_byte(pixel.b);
        }
    } else {
        // Cada píxel está representado por 6 bytes (2 bytes por color)
        for (const auto& pixel : image.pixels) {
            writer.write_word_le(pixel.r);
            writer.write_word_le(pixel.g);
            writer.write_word_le(pixel.b);
        }
    }
    // Escribir los datos binarios en el archivo
    write_binary_file(filename, buffer);
}



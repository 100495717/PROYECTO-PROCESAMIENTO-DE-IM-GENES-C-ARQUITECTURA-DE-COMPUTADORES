#include "imageaos.hpp"
#include "../common/binaryio.hpp"
#include <fstream>
#include <iostream>

ImageAos read_ppm_image_aos(const std::string& filename) {
    // Abrimos el archivo en modo binario para leer
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error: No se pudo abrir el archivo para leer: " << filename << std::endl;
        return {};
    }

    // Cargamos el archivo en un vector de bytes para proceder con la lectura binaria
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    BinaryReader reader(buffer);

    // Leemos y validamos el número mágico (debe ser "P6" para el formato ppm)
    std::string magic_number = reader.read_string();
    if (magic_number != "P6") {
        std::cerr << "Error: El archivo no es un PPM binario (P6)." << std::endl;
        return {};
    }

    // Saltamos espacios en blanco y comentarios después del número mágico
    reader.skip();

    // Leemos el ancho, alto y el valor máximo de color
    int width = reader.read_ascii_int();
    reader.skip();
    int height = reader.read_ascii_int();
    reader.skip();
    int max_color_value = reader.read_ascii_int();

    // Validamos el valor máximo de color
    if (max_color_value != 255 && max_color_value != 65535) {
        std::cerr << "Error: El valor máximo de color no es compatible: " << max_color_value << std::endl;
        return {};
    }

    // Saltamos un solo byte de salto de línea antes de los datos de la imagen
    reader.skip();

    // Inicializamos la imagen con el tamaño y el valor máximo de color
    ImageAos image(width, height, max_color_value);

    image.pixels.resize(static_cast<std::vector<Pixel>::size_type>(width) * static_cast<std::vector<Pixel>::size_type>(height));

    for (auto& pixel : image.pixels) {
        if (max_color_value <= 255) {
            pixel.r = reader.read_uint8();
            pixel.g = reader.read_uint8();
            pixel.b = reader.read_uint8();
        } else {
            pixel.r = reader.read_uint16();
            pixel.g = reader.read_uint16();
            pixel.b = reader.read_uint16();
        }
    }

    return image;
}



void write_ppm_image_aos(const std::string& filename, const ImageAos& img) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        throw std::runtime_error("Error: No se pudo abrir el archivo para escribir: " + filename);
    }

    BinaryWriter writer(ofs);
    // Escribir el encabezado PPM
    writer.write_string("P6\n");
    writer.write_ascii_int(img.width);
    writer.write_string(" ");
    writer.write_ascii_int(img.height);
    writer.write_string("\n");
    writer.write_ascii_int(img.max_color_value);
    writer.write_string("\n");



    for (const auto& pixel : img.pixels) {
        if (img.max_color_value <= 255) {
            writer.write_uint8(static_cast<uint8_t>(pixel.r));
            writer.write_uint8(static_cast<uint8_t>(pixel.g));
            writer.write_uint8(static_cast<uint8_t>(pixel.b));
        } else {
            writer.write_uint16(pixel.r);
            writer.write_uint16(pixel.g);
            writer.write_uint16(pixel.b);
        }
    }
}
#include "header_ppm.hpp"
#include <stdexcept>
#include <iostream>

PPMHeader read_ppm_header(BinaryReader& reader) {
    PPMHeader header;

    // Leer el número mágico (debe ser "P6")
    header.magic = reader.read_ascii_string(2);
    std::cout << "Magic number: " << header.magic << std::endl;
    if (header.magic != "P6") {
        throw std::runtime_error("Formato de archivo PPM no válido (se esperaba 'P6')");
    }

    reader.skip();
    // Leer el ancho, alto y el valor máximo de color
    header.width = reader.read_ascii_int();
    std::cout << "Width: " << header.width << std::endl;
    reader.skip();
    header.height = reader.read_ascii_int();
    std::cout << "Height: " << header.height << std::endl;
    reader.skip();
    header.max_color_value = reader.read_ascii_int();
    std::cout << "Max color value: " << header.max_color_value << std::endl;

    // Nos aseguramos de que el valor máximo de color esté en el rango de valores válidos
    if (header.max_color_value <= 0 || header.max_color_value >= 65536) {
        throw std::runtime_error("Valor máximo de color no válido en el archivo PPM");
    }
    // Saltar el carácter de nueva línea después del encabezado
    reader.skip();

    return header;
}

void write_ppm_header(BinaryWriter& writer, const PPMHeader& header) {
    // Escribimos el número mágico "P6"
    writer.write_ascii_string("P6");
    writer.write_newline();

    // Escribimos la anchura y altura como enteros en ASCII
    writer.write_ascii_int(header.width);
    writer.write_whitespace();
    writer.write_ascii_int(header.height);
    writer.write_newline();

    // Escribimos el valor máximo de color
    writer.write_ascii_int(header.max_color_value);
    writer.write_newline();
}
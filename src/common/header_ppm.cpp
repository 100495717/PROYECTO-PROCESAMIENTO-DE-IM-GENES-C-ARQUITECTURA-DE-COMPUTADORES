#include "header_ppm.hpp"
#include <stdexcept>

PPMHeader read_ppm_header(BinaryReader& reader) {
    PPMHeader header;

    // Leer el número mágico (debe ser "P6")
    header.magic = reader.read_ascii_string(2);
    if (header.magic != "P6") {
        throw std::runtime_error("Formato de archivo PPM no válido (se esperaba 'P6')");
    }

    // Leer el ancho, alto y el valor máximo de color
    header.width = reader.read_ascii_int();
    header.height = reader.read_ascii_int();
    header.max_color_value = reader.read_ascii_int();

    // Nos aseguramos de que el valor máximo de color esté en el rango de valores válidos
    if (header.max_color_value <= 0 || header.max_color_value >= 65536) {
        throw std::runtime_error("Valor máximo de color no válido en el archivo PPM");
    }
    // Saltar el carácter de nueva línea después del encabezado
    reader.skip(1);

    return header;
}

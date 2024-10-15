#ifndef PPM_HEADER_HPP
#define PPM_HEADER_HPP

#include <string>
#include "binaryio.hpp"  // Incluir el BinaryReader

struct PPMHeader {
    std::string magic;
    int width;
    int height;
    int max_color_value;
};

// Función para leer el encabezado PPM
PPMHeader read_ppm_header(BinaryReader& reader);

#endif // PPM_HEADER_HPP


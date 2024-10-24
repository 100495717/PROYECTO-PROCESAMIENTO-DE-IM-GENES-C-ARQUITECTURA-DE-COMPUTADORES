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

// FunciÃ³n para leer el encabezado PPM
PPMHeader read_ppm_header(BinaryReader& reader);

void write_ppm_header(BinaryWriter& writer, const PPMHeader& header);
#endif // PPM_HEADER_HPP
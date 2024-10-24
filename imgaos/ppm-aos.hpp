#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include "imgaos/imageaos.hpp" // Cambiar a la estructura de datos correspondiente
#include "common/header_ppm.hpp" // Imporartamos el BinaryReader y BinaryWriter

// Función para leer un archivo PPM
ImageAos read_ppm(BinaryReader& reader, const PPMHeader& header);

// Función para escribir un archivo PPM
void write_ppm(BinaryWriter& writer, const ImageAos& image6);

#endif // PPM_HPP


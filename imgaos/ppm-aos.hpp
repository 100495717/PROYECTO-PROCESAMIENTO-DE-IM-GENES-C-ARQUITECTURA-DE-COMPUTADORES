#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include "imgaos/imageaos.hpp" // Cambiar a la estructura de datos correspondiente
#include "common/header_ppm.hpp" // Imporartamos el BinaryReader y BinaryWriter

// Función para leer un archivo PPM
ImageAos read_ppm_image(const std::string& filename);

// Función para escribir un archivo PPM
void write_ppm_image(const std::string& filename, const ImageAos& image);

#endif // PPM_HPP


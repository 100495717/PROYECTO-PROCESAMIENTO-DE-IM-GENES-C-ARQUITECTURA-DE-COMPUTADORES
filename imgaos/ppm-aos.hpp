#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include "imageaos.hpp"

// Función para leer un archivo PPM
ImageAos read_ppm_image_aos(const std::string& filename);

// Función para escribir un archivo PPM
void write_ppm_image_aos(const std::string& filename, const ImageAos& image);

#endif // PPM_HPP


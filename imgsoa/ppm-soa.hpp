#ifndef PPM_SOA_HPP
#define PPM_SOA_HPP

#include <string>
#include "imagesoa.hpp"

// Función para leer un archivo PPM
ImageSoa read_ppm_image_soa(const std::string& filename);

// Función para escribir un archivo PPM
void write_ppm_image_soa(const std::string& filename, const ImageSoa& image);

#endif // PPM_SOA_HPP
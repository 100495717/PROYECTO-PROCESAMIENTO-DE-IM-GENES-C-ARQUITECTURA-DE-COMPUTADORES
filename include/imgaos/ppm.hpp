#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include "../src/imgaos/imageaos.hpp" // Cambiar a la estructura de datos correspondiente

// Función para leer un archivo PPM
ImageAos read_ppm(const std::string& filename);

// Función para escribir un archivo PPM
void write_ppm(const std::string& filename, const ImageAos& image);

#endif // PPM_HPP
#ifndef PPM_HPP
#define PPM_HPP

#include <string>


// Ensure that ImageAos is defined in imageaos.hpp
#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include "../src/imgsoa/imagesoa.hpp"




//Función para leer una imagen en formato PPM
ImageSoa read_ppm(const std::string& filename);

//Función para escribir una imagen en formato PPM
void write_ppm(const std::string& filename, const ImageSoa& img);

#endif //PPM_HPP

#endif // IMAGEAOS_HPP



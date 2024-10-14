#ifndef PPMSOA_HPP
#define PPMSOA_HPP

#include <string>



#include "../src/imgsoa/imagesoa.hpp"




//Función para leer una imagen en formato PPM
ImageSoa read_ppm(const std::string& filename);

//Función para escribir una imagen en formato PPM
void write_ppm(const std::string& filename, const ImageSoa& img);

#endif //PPM_HPP

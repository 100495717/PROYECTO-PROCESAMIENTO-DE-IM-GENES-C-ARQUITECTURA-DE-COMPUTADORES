#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <string>
#include <tuple>
#include "pixel-aos.hpp"

// ImageAos  actúa como un "modelo" en memoria de la imagen original, adaptado para permitir un
// procesamiento de imágenes más flexible y eficiente dentro del programa
class ImageAos {
public:
    int width;
    int height;
    int max_color_value;
    std::vector<Pixel> pixels;

    // Constructor por defecto
    ImageAos() : width(0), height(0), max_color_value(255) {}

    // Constructor con parámetros
    ImageAos(int w, int h, int max_val) : width(w), height(h), max_color_value(max_val) {
        pixels.resize(static_cast<std::vector<Pixel>::size_type>(w) * static_cast<std::vector<Pixel>::size_type>(h));
    }

    // Constructor que acepta una ruta de archivo
    explicit ImageAos(const std::string& filename);


};

#endif // IMAGEAOS_HPP
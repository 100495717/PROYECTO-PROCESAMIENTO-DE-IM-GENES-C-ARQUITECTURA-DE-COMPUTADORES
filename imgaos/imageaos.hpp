#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <string>

struct Pixel {
    unsigned char r, g, b;
    // Definir el operador <
    bool operator<(const Pixel& other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }

    // Definir el operador ==
    bool operator==(const Pixel& other) const {
        return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
    }
};

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
        pixels.resize(w * h);
    }

    // Constructor que acepta una ruta de archivo
    ImageAos(const std::string& filename);

    
};

#endif // IMAGEAOS_HPP
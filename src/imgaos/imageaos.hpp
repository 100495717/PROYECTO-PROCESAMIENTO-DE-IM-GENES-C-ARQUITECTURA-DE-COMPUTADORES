#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>

struct Pixel {
    unsigned char r, g, b;
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
};

#endif // IMAGEAOS_HPP
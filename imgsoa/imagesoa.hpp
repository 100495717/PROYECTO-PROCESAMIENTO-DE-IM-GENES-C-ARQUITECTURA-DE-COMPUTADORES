#ifndef IMAGE_SOA_HPP
#define IMAGE_SOA_HPP

#include <vector>
#include <string>
#include <cstdint>

class ImageSoa {
public:
    int width;
    int height;
    int max_color_value;
    std::vector<uint16_t> r; // Vector para los componentes rojos
    std::vector<uint16_t> g; // Vector para los componentes verdes
    std::vector<uint16_t> b; // Vector para los componentes azules

    // Constructor por defecto
    ImageSoa() : width(0), height(0), max_color_value(255) {}

    // Constructor con parámetros
    ImageSoa(int w, int h, int max_val) : width(w), height(h), max_color_value(max_val) {
        r.resize(w * h);
        g.resize(w * h);
        b.resize(w * h);
    }

    // Constructor que acepta una ruta de archivo
    explicit ImageSoa(const std::string& filename);
};

#endif // IMAGE_SOA_HPP

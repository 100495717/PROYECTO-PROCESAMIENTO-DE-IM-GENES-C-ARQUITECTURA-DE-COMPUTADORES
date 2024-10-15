#include "../imgaos/ppm.hpp"
#include "imageaos.hpp"
#include <fstream>
#include <stdexcept>


ImageAos read_ppm_image(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para lectura");
    }

    std::string magic;
    file >> magic;
    if (magic != "P6") {
        throw std::runtime_error("Formato de archivo no válido");
    }

    // Saltar espacios en blanco y comentarios
    

    int width, height, maxval;
    file >> width >> height >> maxval;
    file.ignore(1);

    ImageAos image;
    image.width = width;
    image.height = height;
    image.max_color_value = maxval;
    image.pixels.resize(width * height);

    for (auto& pixel : image.pixels) {
        file.read(reinterpret_cast<char*>(&pixel.r), 1);
        file.read(reinterpret_cast<char*>(&pixel.g), 1);
        file.read(reinterpret_cast<char*>(&pixel.b), 1);
    }

    return image;
}

void write_ppm_image(const std::string& filename, const ImageAos& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "P6\n" << image.width << " " << image.height << "\n" << image.max_color_value << "\n";

    for (const auto& pixel : image.pixels) {
        file.write(reinterpret_cast<const char*>(&pixel.r), 1);
        file.write(reinterpret_cast<const char*>(&pixel.g), 1);
        file.write(reinterpret_cast<const char*>(&pixel.b), 1);
    }
}
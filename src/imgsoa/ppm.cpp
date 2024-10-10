#include "ppm.hpp"
#include <fstream>
#include <stdexcept>
#include "imagesoa.hpp"

ImageSoa read_ppm(const std::string& filename){
    std:: ifstream file(filename, std::ios::binary);
    if(!file){
        throw std::runtime_error("No se pudo abrir el archivo para lectura");
    }

    std::string magic;
    file >> magic;
    if (magic != "P6"){
        throw std::runtime_error("Formato de archivo no válido");
    }

    int width, height, maxval;
    file >> width >> height >> maxval;
    file.ignore(1);

    ImageSoa image;
    image.width = width;
    image.height = height;
    image.max_color_value = maxval;
    image.pixels.resize(width * height);

    for (auto& pixel : image.pixels){
        file.read(reinterpret_cast<char*>(&pixel.r), 1);
        file.read(reinterpret_cast<char*>(&pixel.g), 1);
        file.read(reinterpret_cast<char*>(&pixel.b), 1);
    }
    return image;
}

void write_ppm(const std::string& filename, const ImageSoa& img){
    std::ofstream file(filename, std::ios::binary);
    if (!file){
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "P6\n" << img.width << " " << img.height << "\n" << img.max_color_value << "\n";
    for (const auto& pixel : img.pixels){
        file.write(reinterpret_cast<const char*>(&pixel.r), 1);
        file.write(reinterpret_cast<const char*>(&pixel.g), 1);
        file.write(reinterpret_cast<const char*>(&pixel.b), 1);
    }
#include "ppm-aos.hpp"
#include "imageaos.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>


ImageAos read_ppm(const std::string& filename) {
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

    // Comprobamos si los parámetros de la imagen son válidos
    if (width <= 0 || height <= 0 || maxval <= 0) {
        throw std::runtime_error("Parámetros de imagen no válidos");
    }

    if(maxval < 256){
        // Cada píxel está representado por 3 bytes
        for (int i= 0; i < width*height; ++i){
            unsigned char rgb[3];
            file.read(reinterpret_cast<char*>(rgb), 3);
            image.pixels[i] = {rgb[0], rgb[1], rgb[2]};
        }
    } else {
        //Cada píxel está representado por 6 bytes
        for (int i= 0; i < width*height; ++i){
            unsigned short rgb[3];
            file.read(reinterpret_cast<char*>(rgb), 6);
            image.pixels[i] = {
                                static_cast<unsigned char>(rgb[0] >> 8), 
                                static_cast<unsigned char>(rgb[1] >> 8), 
                                static_cast<unsigned char>(rgb[2] >> 8)};
        }
    }

    return image;
}

void write_ppm(const std::string& filename, const ImageAos& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "P6\n" << image.width << " " << image.height << "\n" << image.max_color_value << "\n";

    if (image.max_color_value < 256) {
        
        //Cada píxel está representado por 3 bytes
        for (const auto& pixel : image.pixels) {
            unsigned char r = static_cast<unsigned char>(pixel.r);
            unsigned char g = static_cast<unsigned char>(pixel.g);
            unsigned char b = static_cast<unsigned char>(pixel.b);
            file.write(reinterpret_cast<char*>(r), 1);
            file.write(reinterpret_cast<char*>(g), 1);
            file.write(reinterpret_cast<char*>(b), 1);
        }
    } else {
        for (const auto& pixel : image.pixels) {
            unsigned short r = pixel.r;
            unsigned short g = pixel.g;
            unsigned short b = pixel.b;
            file.write(reinterpret_cast<const char*>(&r), 2);
            file.write(reinterpret_cast<const char*>(&g), 2);
            file.write(reinterpret_cast<const char*>(&b), 2);
        }
    }
}
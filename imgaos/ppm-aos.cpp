#include "imageaos.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

ImageAos read_ppm_image(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error: No se pudo abrir el archivo para leer: " << filename << std::endl;
        return {};
    }

    std::string header;
    ifs >> header;
    if (header != "P6") {
        std::cerr << "Error: Formato de archivo no soportado: " << header << std::endl;
        return {};
    }

    int width, height, max_color_value;
    ifs >> width >> height >> max_color_value;
    ifs.ignore(256, '\n'); // Ignorar el salto de línea después del valor máximo

    ImageAos img(width, height, max_color_value);
    if (max_color_value <= 255) {
        // Leer 3 bytes (RGB) por cada píxel
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                Pixel p;
                p.r = ifs.get();
                p.g = ifs.get();
                p.b = ifs.get();
                img.pixels[i * width + j] = p;
            }
        }
    } else {
        // Leer 6 bytes (RGB, 2 bytes por color) por cada píxel en formato big-endian
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                Pixel p;
                p.r = (ifs.get() << 8) | ifs.get();
                p.g = (ifs.get() << 8) | ifs.get();
                p.b = (ifs.get() << 8) | ifs.get();
                img.pixels[i * width + j] = p;
            }
        }
    }
    ifs.close();
    return img;
}

void write_ppm_image(const std::string& filename, const ImageAos& img) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error: No se pudo abrir el archivo para escribir: " << filename << std::endl;
        return;
    }

    ofs << "P6\n" << img.width << " " << img.height << "\n" << img.max_color_value << "\n";
    if (img.max_color_value <= 255) {
        // Escribir 3 bytes (RGB) por cada píxel
        for (const auto& pixel : img.pixels) {
            ofs.put(static_cast<char>(pixel.r));
            ofs.put(static_cast<char>(pixel.g));
            ofs.put(static_cast<char>(pixel.b));
        }
    } else {
        // Escribir 6 bytes (RGB, 2 bytes por color) por cada píxel en formato big-endian
        for (const auto& pixel : img.pixels) {
            ofs.put(static_cast<char>(pixel.r >> 8));
            ofs.put(static_cast<char>(pixel.r & 0xFF));
            ofs.put(static_cast<char>(pixel.g >> 8));
            ofs.put(static_cast<char>(pixel.g & 0xFF));
            ofs.put(static_cast<char>(pixel.b >> 8));
            ofs.put(static_cast<char>(pixel.b & 0xFF));
        }
    }
    ofs.close();
}
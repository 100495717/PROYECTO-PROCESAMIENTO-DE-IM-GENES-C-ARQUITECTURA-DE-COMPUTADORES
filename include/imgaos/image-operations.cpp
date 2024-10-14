#include "image-operations.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

void print_image_info(const ImageAos& img){
    std::cout << "Ancho: " << img.width << std::endl;
    std::cout << "Alto: " << img.height << std::endl;
    std::cout << "Píxeles totales: " << img.pixels.size() << std::endl;

}

void max_level(ImageAos& img, int maxlevel){
    for (auto& pixel : img.pixels){
        pixel.r = std::min(pixel.r, static_cast<unsigned char>(maxlevel));
        pixel.g = std::min(pixel.g, static_cast<unsigned char>(maxlevel));
        pixel.b = std::min(pixel.b, static_cast<unsigned char>(maxlevel));
    }
}

void resize_image(ImageAos& img, int width, int height){
    std::vector<Pixel> new_pixels(width * height);
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_pixels[y * width + x] = img.pixels[py * img.width + px];
            
        }
    }
    img.width = width;
    img.height = height;
    img.pixels = std::move(new_pixels);
}

void cut_least_freq(ImageAos& img, int n) {
    std::map<Pixel, int> freq_map;
    for (const auto& pixel : img.pixels) {
        freq_map[pixel]++;
    }

    std::vector<std::pair<Pixel, int>> freq_vec(freq_map.begin(), freq_map.end());
    std::sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    std::vector<Pixel> least_freq_pixels;
    for (int i = 0; i < n && i < freq_vec.size(); ++i) {
        least_freq_pixels.push_back(freq_vec[i].first);
    }

    for (auto& pixel : img.pixels) {
        if (std::find(least_freq_pixels.begin(), least_freq_pixels.end(), pixel) != least_freq_pixels.end()) {
            pixel = {0, 0, 0}; // Replace with black
        }
    }
}

void compress_image(const ImageAos& img, const std::string& output) {
    // Implementación de compresión de imagen (puede ser cualquier algoritmo de compresión)
    // Aquí solo se muestra un ejemplo simple de guardar la imagen en un formato comprimido
    std::ofstream file(output, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Cannot open file " + output);
    }

    // Escribir un encabezado simple para el archivo comprimido
    file << "CPPM\n" << img.width << " " << img.height << "\n";

    // Escribir los datos de los píxeles comprimidos (aquí simplemente se copian los datos)
    for (const auto& pixel : img.pixels) {
        file.put(pixel.r);
        file.put(pixel.g);
        file.put(pixel.b);
    }

    if (!file) {
        throw std::runtime_error("Error: Failed to write compressed pixel data");
    }
}
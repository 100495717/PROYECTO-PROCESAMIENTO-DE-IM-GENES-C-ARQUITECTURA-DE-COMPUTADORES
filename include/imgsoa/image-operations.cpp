#include "../src/imgsoa/imagesoa.hpp"
#include "image-operations.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>

void print_image_info_soa(const ImageSoa& img) {
    std::cout << "Image width: " << img.width << std::endl;
    std::cout << "Image height: " << img.height << std::endl;
    std::cout << "Total pixels: " << img.width * img.height << std::endl;
}

void max_level_soa(ImageSoa& img, int maxlevel) {
    for (int i = 0; i < img.width * img.height; ++i) {
        img.r[i] = std::min(img.r[i], static_cast<unsigned char>(maxlevel));
        img.g[i] = std::min(img.g[i], static_cast<unsigned char>(maxlevel));
        img.b[i] = std::min(img.b[i], static_cast<unsigned char>(maxlevel));
    }
}

void resize_image_soa(ImageSoa& img, int width, int height) {
    std::vector<unsigned char> new_r(width * height);
    std::vector<unsigned char> new_g(width * height);
    std::vector<unsigned char> new_b(width * height);
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_r[y * width + x] = img.r[py * img.width + px];
            new_g[y * width + x] = img.g[py * img.width + px];
            new_b[y * width + x] = img.b[py * img.width + px];
        }
    }
    img.width = width;
    img.height = height;
    img.r = std::move(new_r);
    img.g = std::move(new_g);
    img.b = std::move(new_b);
}

void cut_least_freq_soa(ImageSoa& img, int n) {
    std::map<std::tuple<unsigned char, unsigned char, unsigned char>, int> freq_map;
    for (int i = 0; i < img.width * img.height; ++i) {
        freq_map[std::make_tuple(img.r[i], img.g[i], img.b[i])]++;
    }

    std::vector<std::pair<std::tuple<unsigned char, unsigned char, unsigned char>, int>> freq_vec(freq_map.begin(), freq_map.end());
    std::sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    std::vector<std::tuple<unsigned char, unsigned char, unsigned char>> least_freq_pixels;
    for (int i = 0; i < n && i < freq_vec.size(); ++i) {
        least_freq_pixels.push_back(freq_vec[i].first);
    }

    for (int i = 0; i < img.width * img.height; ++i) {
        if (std::find(least_freq_pixels.begin(), least_freq_pixels.end(), std::make_tuple(img.r[i], img.g[i], img.b[i])) != least_freq_pixels.end()) {
            img.r[i] = 0;
            img.g[i] = 0;
            img.b[i] = 0;
        }
    }
}

void compress_image_soa(const ImageSoa& img, const std::string& output) {
    std::ofstream file(output, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "CPPM\n" << img.width << " " << img.height << "\n";

    for (int i = 0; i < img.width * img.height; ++i) {
        file.put(img.r[i]);
        file.put(img.g[i]);
        file.put(img.b[i]);
    }

    if (!file) {
        throw std::runtime_error("Error al escribir los datos de los píxeles");
    }
}
// image-operation-aos.hpp
#ifndef IMAGE_OPERATION_AOS_HPP
#define IMAGE_OPERATION_AOS_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include "../common/binaryio.hpp"

struct Pixel {
    uint16_t r;
    uint16_t g;
    uint16_t b;

    bool operator<(const Pixel& other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }

    bool operator==(const Pixel& other) const {
        return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
    }
};

namespace std {
    template <>
    struct hash<Pixel> {
        std::size_t operator()(const Pixel& pixel) const {
            return ((std::hash<uint16_t>()(pixel.r) ^ (std::hash<uint16_t>()(pixel.g) << 1)) >> 1) ^ (std::hash<uint16_t>()(pixel.b) << 1);
        }
    };
}

struct ImageAos {
    int width;
    int height;
    int max_color_value;
    std::vector<Pixel> pixels;

    ImageAos() : width(0), height(0), max_color_value(255) {}
};

// Declaración de funciones
void load_image_from_file(const std::string& filePath, ImageAos& img);
void write_image_to_file(const std::string& filePath, const ImageAos& img);
void print_image_info(const ImageAos& img);
template <typename T>
T clamp(T value, T min, T max);
void max_level(ImageAos& img, int maxlevel);
void print_pixels(const std::vector<Pixel>& pixels);
void resize_image(ImageAos& img, int width, int height);
double color_distance(const Pixel& a, const Pixel& b);
Pixel find_closest_pixel(const Pixel& target, const std::vector<Pixel>& pixels);
void cut_freq(ImageAos& img, int n);
void compress_image(const ImageAos& img, const std::string& output);

#endif 
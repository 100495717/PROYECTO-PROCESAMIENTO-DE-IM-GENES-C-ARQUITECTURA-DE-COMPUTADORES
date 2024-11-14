/*#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cstdint>
#include <functional>

struct Pixel {
  
    uint16_t r = 0; // Cambiado a uint16_t para soportar 65535
    uint16_t g = 0;
    uint16_t b = 0;


    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool operator<(const Pixel& other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }
};

// Especialización de std::hash para Pixel
template <>
struct std::hash<Pixel> {
    std::size_t operator()(const Pixel& p) const noexcept{
        return (static_cast<std::size_t>(p.r) << 16) |
               (static_cast<std::size_t>(p.g) << 8) |
               static_cast<std::size_t>(p.b);
    }
};

#endif */
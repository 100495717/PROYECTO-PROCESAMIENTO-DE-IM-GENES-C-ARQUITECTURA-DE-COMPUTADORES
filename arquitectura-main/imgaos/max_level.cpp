#include "imgaos/imageaos.hpp"

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace imgaos {

  constexpr int MAX_MAXLEVEL = 65536;

  void AOS::max_level(int new_max_level) {
    if (new_max_level <= 0 || new_max_level >= MAX_MAXLEVEL) {
      throw std::invalid_argument("Invalid maxlevel");
    }
    if (new_max_level <= UINT8_MAX) {
      if (maxlevel <= UINT8_MAX) {
        max_level_generic<uint8_t, uint8_t>(new_max_level);
      } else {
        max_level_generic<uint16_t, uint8_t>(new_max_level);
      }
      type = Type::UINT8;
    } else {
      if (maxlevel <= UINT8_MAX) {
        max_level_generic<uint8_t, uint16_t>(new_max_level);
      } else {
        max_level_generic<uint16_t, uint16_t>(new_max_level);
      }
      type = Type::UINT16;
    }
  }

  template <typename T, typename U>
  void AOS::max_level_generic(int level) {
    if constexpr (std::is_same_v<T, U>) {
      for (auto & my_pixel : std::get<std::vector<pixel<T>>>(data)) {
        T new_r  = static_cast<T>(my_pixel.getR() * level / maxlevel);
        T new_g  = static_cast<T>(my_pixel.getG() * level / maxlevel);
        T new_b  = static_cast<T>(my_pixel.getB() * level / maxlevel);
        my_pixel = pixel<T>(new_r, new_g, new_b);
      }
      maxlevel = level;
    } else {
      std::vector<pixel<U>> vector_data;
      vector_data.reserve(std::get<std::vector<pixel<T>>>(data).size());
      for (auto & my_pixel : std::get<std::vector<pixel<T>>>(data)) {
        pixel<U> const new_pixel(static_cast<U>(my_pixel.getR() * level / maxlevel),
                                 static_cast<U>(my_pixel.getG() * level / maxlevel),
                                 static_cast<U>(my_pixel.getB() * level / maxlevel));
        vector_data.push_back(new_pixel);
      }
      data     = std::move(vector_data);
      maxlevel = level;
    }
  }
}  // namespace imgaos
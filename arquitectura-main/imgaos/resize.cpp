#include "../common/aux.hpp"
#include "imageaos.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace imgaos {

  void AOS::resize(int new_width, int new_height) {
    switch (type) {
      case Type::UINT8:
        resize_generic<uint8_t>(common::Width(new_width), common::Height(new_height));
        break;
      case Type::UINT16:
        resize_generic<uint16_t>(common::Width(new_width), common::Height(new_height));
        break;
    }
  }

  template <typename T>
  void AOS::resize_generic(common::Width new_width, common::Height new_height) {
    if (new_width.getValue() <= 0) { throw std::invalid_argument("Invalid width"); }
    if (new_height.getValue() <= 0) { throw std::invalid_argument("Invalid height"); }

    common::Size const old_size = common::Size(common::Height(height), common::Width(width));
    width                       = new_width.getValue();
    height                      = new_height.getValue();
    auto & old_vector_data      = std::get<std::vector<pixel<T>>>(data);

    std::vector<pixel<T>> new_vector_data;
    new_vector_data.reserve(static_cast<size_t>(width) * static_cast<size_t>(height));

    for (int new_y = 0; new_y < height; new_y++) {
      for (int new_x = 0; new_x < width; new_x++) {
        pixel<T> const new_pixel = interpolate_pixel<T>(old_vector_data, old_size, new_x, new_y);
        new_vector_data.push_back(new_pixel);
      }
    }
    data = std::move(new_vector_data);
  }

  template <typename T>
  pixel<T> AOS::interpolate_pixel(std::vector<pixel<T>> const & old_vector_data,
                                  common::Size old_size, int new_x, int new_y) {
    float const old_x = static_cast<float>(new_x * (old_size.getWidth().getValue() - 1)) /
                        static_cast<float>(width - 1);
    float const old_y = static_cast<float>(new_y * (old_size.getHeight().getValue() - 1)) /
                        static_cast<float>(height - 1);
    int const x_l = static_cast<int>(std::floor(old_x));
    int const x_h = static_cast<int>(std::ceil(old_x));
    int const y_l = static_cast<int>(std::floor(old_y));
    int const y_h = static_cast<int>(std::ceil(old_y));

    pixel<T> const pixel_ll =
        old_vector_data[static_cast<size_t>(y_l) *
                            static_cast<size_t>(old_size.getWidth().getValue()) +
                        static_cast<size_t>(x_l)];
    pixel<T> const pixel_hl =
        old_vector_data[static_cast<size_t>(y_l) *
                            static_cast<size_t>(old_size.getWidth().getValue()) +
                        static_cast<size_t>(x_h)];
    pixel<T> const pixel_lh =
        old_vector_data[static_cast<size_t>(y_h) *
                            static_cast<size_t>(old_size.getWidth().getValue()) +
                        static_cast<size_t>(x_l)];
    pixel<T> const pixel_hh =
        old_vector_data[static_cast<size_t>(y_h) *
                            static_cast<size_t>(old_size.getWidth().getValue()) +
                        static_cast<size_t>(x_h)];

    auto const red =
        static_cast<T>((pixel_ll.getR() + pixel_hl.getR() + pixel_lh.getR() + pixel_hh.getR()) / 4);
    auto const green =
        static_cast<T>((pixel_ll.getG() + pixel_hl.getG() + pixel_lh.getG() + pixel_hh.getG()) / 4);
    auto const blue =
        static_cast<T>((pixel_ll.getB() + pixel_hl.getB() + pixel_lh.getB() + pixel_hh.getB()) / 4);

    return pixel<T>(red, green, blue);
  }
}  // namespace imgaos
#include "imageaos.hpp"

#include "../common/aux.hpp"
#include "../common/parsebinary.hpp"
#include "../common/validatefile.hpp"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

namespace imgaos {

  int AOS::getWidth() const {
    return width;
  }

  int AOS::getHeight() const {
    return height;
  }

  int AOS::getMaxLevel() const {
    return maxlevel;
  }

  AOS::AOS(std::vector<uint8_t> const & binary) {
    std::vector<std::string> const tokens = common::ParseBinary::parse(binary);

    common::validateFile::validate_magic_number(tokens[0]);
    width    = common::validateFile::validate_width(tokens[1]);
    height   = common::validateFile::validate_height(tokens[2]);
    maxlevel = common::validateFile::validate_maxlevel(tokens[3]);

    if (maxlevel <= UINT8_MAX) {
      type = Type::UINT8;
    } else {
      type = Type::UINT16;
    }

    size_t const bytes_per_pixel = static_cast<size_t>(type) * 3;
    auto const total_pixels      = static_cast<size_t>(width) * static_cast<size_t>(height);
    size_t const total_bytes     = total_pixels * bytes_per_pixel;

    common::validateFile::validate_pixels(tokens[4], total_bytes);

    if (type == Type::UINT8) {
      process_uint8_pixels(tokens[4], total_pixels);
    } else {
      process_uint16_pixels(tokens[4], total_pixels);
    }
  }

  void AOS::process_uint8_pixels(std::string const & pixel_data, size_t total_pixels) {
    std::vector<pixel<uint8_t>> vector_data;
    vector_data.reserve(total_pixels);

    for (size_t i = 0; i < total_pixels; i++) {
      pixel<uint8_t> const new_pixel(
          common::Red<uint8_t>(static_cast<uint8_t>(pixel_data[i * 3])),
          common::Green<uint8_t>(static_cast<uint8_t>(pixel_data[i * 3 + 1])),
          common::Blue<uint8_t>(static_cast<uint8_t>(pixel_data[i * 3 + 2])));
      vector_data.push_back(new_pixel);
    }

    data = std::move(vector_data);
  }

  void AOS::process_uint16_pixels(std::string const & pixel_data, size_t total_pixels) {
    std::vector<pixel<uint16_t>> vector_data;
    vector_data.reserve(total_pixels);

    for (size_t i = 0; i < total_pixels; i++) {
      pixel<uint16_t> const new_pixel(common::Red<uint16_t>(static_cast<uint16_t>(
                                          (pixel_data[i * 6] << 8) | pixel_data[i * 6 + 1])),
                                      common::Green<uint16_t>(static_cast<uint16_t>(
                                          (pixel_data[i * 6 + 2] << 8) | pixel_data[i * 6 + 3])),
                                      common::Blue<uint16_t>(static_cast<uint16_t>(
                                          (pixel_data[i * 6 + 4] << 8) | pixel_data[i * 6 + 5])));
      vector_data.push_back(new_pixel);
    }

    data = std::move(vector_data);
  }

  std::vector<uint8_t> AOS::toBinary() const {
    std::string const header = "P6\n" + std::to_string(width) + " " + std::to_string(height) +
                               "\n" + std::to_string(maxlevel) + "\n";
    std::vector<uint8_t> binary;

    size_t const pixel_size       = static_cast<size_t>(type) * 3;
    size_t const number_of_pixels = static_cast<size_t>(width) * static_cast<size_t>(height);
    binary.reserve(header.size() + number_of_pixels * pixel_size);

    binary.insert(binary.begin(), header.begin(), header.end());

    write_pixels(binary);

    return binary;
  }

  void AOS::write_pixels(std::vector<uint8_t> & binary) const {
    if (type == Type::UINT8) {
      for (auto const & my_pixel : std::get<std::vector<pixel<uint8_t>>>(data)) {
        my_pixel.write_to_binary(binary);
      }
    } else {
      for (auto const & my_pixel : std::get<std::vector<pixel<uint16_t>>>(data)) {
        my_pixel.write_to_binary(binary);
      }
    }
  }
}  // namespace imgaos
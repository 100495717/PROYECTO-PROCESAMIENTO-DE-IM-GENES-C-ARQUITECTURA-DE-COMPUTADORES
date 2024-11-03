/*#include "imageaos.hpp"

#include <iostream>
#include <unordered_map>

namespace imgaos {
  template <typename T>
  std::vector<uint8_t> AOS::compress_generic() const {
    auto pixel_data = std::get<std::vector<pixel<T>>>(data);
    std::unordered_map<pixel<T>, std::pair<size_t, std::forward_list<size_t>>, PixelHash<T>> freq;
    freq.reserve(pixel_data.size() / 2);
    for (size_t i = 0; i < pixel_data.size(); i++) {
      freq[pixel_data[i]].first++;
      freq[pixel_data[i]].second.push_front(i);
    }
    std::string const header = "C6 " + std::to_string(width) + " " + std::to_string(height) + " " +
                               std::to_string(maxlevel) + " " + std::to_string(freq.size()) + "\n";
    //! TODO pre allocate memory
    std::vector<uint8_t> binary(header.begin(), header.end());

    size_t table_index = 0;
    for (auto const & [pixel, freqs] : freq) {
      pixel.write_to_binary(binary);
      for (auto const & position : freqs.second) {
        //! TODO write on binary[position+offset] the table_index
      }

      table_index++;
    }

    return binary;
  }

  std::vector<uint8_t> AOS::compress() const {
    switch (type) {
      case Type::UINT8:
        return compress_generic<uint8_t>();
      case Type::UINT16:
        return compress_generic<uint16_t>();
    }
    // unreachable
    return {};
  }

}  // namespace imgaos*/
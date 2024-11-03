#include "aux.hpp"

namespace common {

  Width::Width(int value) : value(value) { }

  int Width::getValue() const {
    return value;
  }

  Height::Height(int value) : value(value) { }

  int Height::getValue() const {
    return value;
  }

  Size::Size(Height height, Width width) : height(height), width(width) { }

  Height Size::getHeight() const {
    return height;
  }

  Width Size::getWidth() const {
    return width;
  }

}  // namespace common
#ifndef AUX_HPP
#define AUX_HPP

namespace common {
  class Width {
    public:
      explicit Width(int value);
      [[nodiscard]] int getValue() const;

    private:
      int value;
  };

  class Height {
    public:
      explicit Height(int value);
      [[nodiscard]] int getValue() const;

    private:
      int value;
  };

  class Size {
    public:
      explicit Size(Height height, Width width);
      [[nodiscard]] Height getHeight() const;
      [[nodiscard]] Width getWidth() const;

    private:
      Height height;
      Width width;
  };

  template <typename T>
  struct Red {
    public:
      Red(T value);
      [[nodiscard]] T getValue() const;

    private:
      T value;
  };

  template <typename T>
  struct Green {
    public:
      Green(T value);
      [[nodiscard]] T getValue() const;

    private:
      T value;
  };

  template <typename T>
  struct Blue {
    public:
      Blue(T value);
      [[nodiscard]] T getValue() const;

    private:
      T value;
  };

  // Implementation
  template <typename T>
  Red<T>::Red(T value) : value(value) { }

  template <typename T>
  T Red<T>::getValue() const {
    return value;
  }

  template <typename T>
  Green<T>::Green(T value) : value(value) { }

  template <typename T>
  T Green<T>::getValue() const {
    return value;
  }

  template <typename T>
  Blue<T>::Blue(T value) : value(value) { }

  template <typename T>
  T Blue<T>::getValue() const {
    return value;
  }
}  // namespace common
#endif
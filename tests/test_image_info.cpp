// tests/test_image_info.cpp
#include "../imgaos/image-operations-aos.hpp"
#include <iostream>

int main() {
    // Llama a la función image-info y muestra el resultado
    ImageAos image("../input/lake-large.ppm");
    print_image_info(image);
    return 0;
}
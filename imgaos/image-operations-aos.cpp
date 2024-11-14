#include "image-operations-aos.hpp"
#include "../common/binaryio.hpp"
#include <iostream>
#include <fstream>
#include "pixel-aos.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <sstream>




void load_image_from_file(const std::string& filePath, ImageAos& img) {
    // Leer el archivo binario completo en un buffer de bytes
    std::vector<uint8_t> buffer = BinaryIO::readBinaryFile(filePath);

    size_t offset = 0;

    // Leer el número mágico y verificar que sea "P6"
    std::string magic_number(buffer.begin(), buffer.begin() + 2);
    if (magic_number != "P6") {
        throw std::runtime_error("Formato de archivo incorrecto: no es un archivo PPM P6");
    }
    offset += 2;

    // Saltar espacios en blanco después del número mágico
    while (isspace(buffer[offset])) {
        ++offset;
    }

    // Leer la anchura de la imagen
    std::stringstream width_stream;
    while (isdigit(buffer[offset])) {
        width_stream << buffer[offset++];
    }
    img.width = std::stoi(width_stream.str());

    // Saltar espacios en blanco después de la anchura
    while (isspace(buffer[offset])) {
        ++offset;
    }

    // Leer la altura de la imagen
    std::stringstream height_stream;
    while (isdigit(buffer[offset])) {
        height_stream << buffer[offset++];
    }
    img.height = std::stoi(height_stream.str());

    // Saltar espacios en blanco después de la altura
    while (isspace(buffer[offset])) {
        ++offset;
    }

    // Leer el valor máximo de color
    std::stringstream max_color_value_stream;
    while (isdigit(buffer[offset])) {
        max_color_value_stream << buffer[offset++];
    }
    img.max_color_value = std::stoi(max_color_value_stream.str());

    // Verificar que el valor máximo de color sea válido
    if (img.max_color_value <= 0 || img.max_color_value >= 65536) {
        throw std::runtime_error("Valor máximo de color no válido en el archivo PPM");
    }

    // Saltar el único carácter en blanco después del valor máximo de color
    if (!isspace(buffer[offset])) {
        throw std::runtime_error("Formato de archivo incorrecto: falta el carácter en blanco después del valor máximo de color");
    }
    ++offset;

    // Calcular el tamaño de cada píxel en bytes (3 bytes si max_color_value <= 255, o 6 bytes si es mayor)
    int pixel_size = (img.max_color_value <= 255) ? 3 : 6;
    img.pixels.resize(static_cast<size_t>(img.width) * static_cast<size_t>(img.height));

    // Leer los píxeles de la imagen
    for (size_t i = 0; i < img.pixels.size(); ++i) {
        if (pixel_size == 3) {
            // Leer 1 byte por componente (RGB)
            img.pixels[i].r = buffer[offset++];
            img.pixels[i].g = buffer[offset++];
            img.pixels[i].b = buffer[offset++];
        } else {
            // Leer 2 bytes por componente (RGB), en formato little-endian
            img.pixels[i].r = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
            img.pixels[i].g = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
            img.pixels[i].b = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
        }
    }
}


void write_image_to_file(const std::string& filePath, const ImageAos& img) {
    // Crear un buffer para almacenar todos los datos del archivo
    std::vector<uint8_t> buffer;

    // Escribir el número mágico "P6" al inicio
    buffer.push_back('P');
    buffer.push_back('6');
    buffer.push_back('\n');

    // Escribir la anchura y altura como texto, seguidos de un salto de línea
    std::string width_str = std::to_string(img.width);
    buffer.insert(buffer.end(), width_str.begin(), width_str.end());
    buffer.push_back(' ');

    std::string height_str = std::to_string(img.height);
    buffer.insert(buffer.end(), height_str.begin(), height_str.end());
    buffer.push_back('\n');

    // Escribir el valor máximo de color, seguido de un salto de línea
    std::string max_color_str = std::to_string(img.max_color_value);
    buffer.insert(buffer.end(), max_color_str.begin(), max_color_str.end());
    buffer.push_back('\n');

    // Escribir los datos de los píxeles en formato RGB
    if (img.max_color_value <= 255) {
        // Si el valor máximo de color es <= 255, cada componente se guarda en 1 byte
        for (const Pixel& pixel : img.pixels) {
            buffer.push_back(static_cast<uint8_t>(pixel.r));
            buffer.push_back(static_cast<uint8_t>(pixel.g));
            buffer.push_back(static_cast<uint8_t>(pixel.b));
        }
    } else {
        // Si el valor máximo de color es > 255, cada componente se guarda en 2 bytes (little-endian)
        for (const Pixel& pixel : img.pixels) {
            buffer.push_back(static_cast<uint8_t>(pixel.r & 0xFF));
            buffer.push_back(static_cast<uint8_t>((pixel.r >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>(pixel.g & 0xFF));
            buffer.push_back(static_cast<uint8_t>((pixel.g >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>(pixel.b & 0xFF));
            buffer.push_back(static_cast<uint8_t>((pixel.b >> 8) & 0xFF));
        }
    }

    // Escribir el buffer al archivo usando BinaryIO
    BinaryIO::writeBinaryFile(filePath, buffer);
}

void print_image_info(const ImageAos& img){
    std::cout << "Ancho: " << img.width << std::endl;
    std::cout << "Alto: " << img.height << std::endl;
    std::cout << "Valor máximo de color: " << img.max_color_value << std::endl;
    std::cout << "Píxeles totales: " << img.pixels.size() << std::endl;

}
template <typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void max_level(ImageAos& img, int maxlevel) {
    // Verificar si el nivel máximo es válido
    if (maxlevel <= 0 || maxlevel > 65535) {
        throw std::invalid_argument("El nivel máximo debe ser mayor a cero y menor a 65535.");
    }

    // Factor de escala
    double scale = static_cast<double>(maxlevel) / img.max_color_value;

    // Verificar si necesitamos cambiar la representación de los píxeles
    bool fromOneToTwoBytes = (img.max_color_value <= 255 && maxlevel > 255);
    bool fromTwoToOneBytes = (img.max_color_value > 255 && maxlevel <= 255);

    // Crear un nuevo vector de píxeles si es necesario cambiar la representación
    std::vector<Pixel> new_pixels;
    if (fromOneToTwoBytes || fromTwoToOneBytes) {
        new_pixels.resize(static_cast<std::vector<Pixel>::size_type>(img.width) * static_cast<std::vector<Pixel>::size_type>(img.height));
    }

    // Iterar sobre los píxeles de la imagen
    for (size_t i = 0; i < img.pixels.size(); ++i) {
        int originalRed = img.pixels[i].r;
        int originalGreen = img.pixels[i].g;
        int originalBlue = img.pixels[i].b;

        // Escalar los valores de los canales RGB con el nuevo valor máximo
        int newRed = static_cast<int>(std::round(originalRed * scale));
        int newGreen = static_cast<int>(std::round(originalGreen * scale));
        int newBlue = static_cast<int>(std::round(originalBlue * scale));

        if (fromOneToTwoBytes) {
            // Convertir a 2 bytes por canal
            new_pixels[i].r = static_cast<uint16_t>(newRed);
            new_pixels[i].g = static_cast<uint16_t>(newGreen);
            new_pixels[i].b = static_cast<uint16_t>(newBlue);
        } else if (fromTwoToOneBytes) {
            // Convertir a 1 byte por canal
            new_pixels[i].r = static_cast<uint8_t>(newRed & 0xFF);
            new_pixels[i].g = static_cast<uint8_t>(newGreen & 0xFF);
            new_pixels[i].b = static_cast<uint8_t>(newBlue & 0xFF);
        } else {
            // Mantener la representación original
            img.pixels[i].r = static_cast<uint8_t>(newRed);
            img.pixels[i].g = static_cast<uint8_t>(newGreen);
            img.pixels[i].b = static_cast<uint8_t>(newBlue);
        }
    }

    // Actualizar el nivel máximo de color de la imagen
    img.max_color_value = maxlevel;

    // Si cambiamos la representación, actualizamos los píxeles
    if (fromOneToTwoBytes || fromTwoToOneBytes) {
        img.pixels = std::move(new_pixels);
    }
}
void print_pixels(const std::vector<Pixel>& pixels) {
    for (const auto& pixel : pixels) {
        std::cout << "Pixel(" << pixel.r << ", " << pixel.g << ", " << pixel.b << ")" << std::endl;
    }
}

void resize_image(ImageAos& img, int width, int height){
    // Comprobamos si las dimensiones proporcionadas son válidas
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Las dimensiones de la imagen deben ser mayores a cero.");
    }
    // Crear un nuevo vector de píxeles con las dimensiones de la nueva imagen
    std::vector<Pixel> new_pixels(static_cast<std::vector<Pixel>::size_type>(width) * static_cast<std::vector<Pixel>::size_type>(height));
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_pixels[static_cast<std::vector<Pixel>::size_type>(y) * static_cast<std::vector<Pixel>::size_type>(width) + static_cast<std::vector<Pixel>::size_type>(x)] = img.pixels[static_cast<std::vector<Pixel>::size_type>(py) * static_cast<std::vector<Pixel>::size_type>(img.width) + static_cast<std::vector<Pixel>::size_type>(px)];
        }
    }
    // Actualizar las dimensiones y los píxeles de la imagen
    img.width = width;
    img.height = height;
    img.pixels = std::move(new_pixels);
}

// Función auxiliar para calcular la distancia euclídea entre dos píxeles
double color_distance(const Pixel& a, const Pixel& b) {
    return std::sqrt(std::pow(a.r - b.r, 2) + std::pow(a.g - b.g, 2) + std::pow(a.b - b.b, 2));
}

// Función para encontrar el píxel más cercano
Pixel find_closest_pixel(const Pixel& target, const std::vector<Pixel>& pixels) {
    double min_distance = std::numeric_limits<double>::max();
    Pixel closest = target;
    // Iterar sobre los píxeles y encontrar el más cercano al objetivo
    for (const auto& pixel : pixels) {
        double distance = color_distance(target, pixel);
        if (distance < min_distance) {
            min_distance = distance;
            closest = pixel;
        }
    }
    return closest;
}

void cut_freq(ImageAos& img, int n) {
    // Paso 1: Determinar la frecuencia de cada color
    std::unordered_map<Pixel, int> freq_map;
    for (const auto& pixel : img.pixels) {
        freq_map[pixel]++;
    }

    // Comprobamos si 'n' es válido
    if (n <= 0) {
        throw std::invalid_argument("El número de colores a eliminar debe ser mayor que cero.");
    }
    if (static_cast<size_t>(n) > freq_map.size()) {
        throw std::invalid_argument("El número de colores a eliminar no puede ser mayor que el número de colores únicos en la imagen.");
    }

    // Paso 2: Identificar los n colores menos frecuentes
    std::vector<std::pair<Pixel, int>> freq_vec(freq_map.begin(), freq_map.end());
    std::partial_sort(freq_vec.begin(), freq_vec.begin() + n, freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Crear un conjunto de los colores menos frecuentes
    std::unordered_set<Pixel> least_freq_pixels;
    for (int i = 0; i < n && static_cast<size_t>(i) < freq_vec.size(); ++i) {
        least_freq_pixels.insert(freq_vec[static_cast<std::vector<std::pair<Pixel, int>>::size_type>(i)].first);
    }

    // Paso 3: Reemplazar los colores menos frecuentes por el color más cercano restante
    std::vector<Pixel> remaining_pixels;
    for (const auto& [pixel, _] : freq_map) {
        if (least_freq_pixels.find(pixel) == least_freq_pixels.end()) {
            remaining_pixels.push_back(pixel);
        }
    }

    // Crear un mapa de reemplazo para evitar cálculos redundantes
    std::unordered_map<Pixel, Pixel> replacement_map;
    for (const auto& pixel : least_freq_pixels) {
        replacement_map[pixel] = find_closest_pixel(pixel, remaining_pixels);
    }

    // Reemplazar los colores menos frecuentes en la imagen
    for (auto& pixel : img.pixels) {
        if (replacement_map.find(pixel) != replacement_map.end()) {
            pixel = replacement_map[pixel];
        }
    }
}

void compress_image(const ImageAos& img, const std::string& output) {
    std::vector<uint8_t> buffer;

    // Escribir el encabezado (por ejemplo, "C6 ancho alto max_color_value")
    std::string header = "C6 " + std::to_string(img.width) + " " + std::to_string(img.height) + " " + std::to_string(img.max_color_value) + " ";
    buffer.insert(buffer.end(), header.begin(), header.end());

    // Crear un mapa de frecuencia de colores y una lista de colores únicos
    std::map<Pixel, int> freq_map;
    std::vector<Pixel> color_list;
    int color_index = 0;
    for (const auto& pixel : img.pixels) {
        if (freq_map.find(pixel) == freq_map.end()) {
            freq_map[pixel] = color_index++;
            color_list.push_back(pixel);
        }
    }

    // Escribir el número de colores únicos
    std::string color_count = std::to_string(color_list.size()) + "\n";
    buffer.insert(buffer.end(), color_count.begin(), color_count.end());

    // Escribir la tabla de colores
    for (const auto& pixel : color_list) {
        if (img.max_color_value <= 255) {
            buffer.push_back(static_cast<uint8_t>(pixel.r));
            buffer.push_back(static_cast<uint8_t>(pixel.g));
            buffer.push_back(static_cast<uint8_t>(pixel.b));
        } else {
            buffer.push_back(static_cast<uint8_t>(pixel.r >> 8));
            buffer.push_back(static_cast<uint8_t>(pixel.r & 0xFF));
            buffer.push_back(static_cast<uint8_t>(pixel.g >> 8));
            buffer.push_back(static_cast<uint8_t>(pixel.g & 0xFF));
            buffer.push_back(static_cast<uint8_t>(pixel.b >> 8));
            buffer.push_back(static_cast<uint8_t>(pixel.b & 0xFF));
        }
    }

    // Escribir los índices de los colores
    int index_size = (color_list.size() < 256) ? 1 : (color_list.size() < 65536) ? 2 : 4;
    for (const auto& pixel : img.pixels) {
        int index = freq_map[pixel];
        for (int i = 0; i < index_size; ++i) {
            buffer.push_back(static_cast<uint8_t>((index >> (i * 8)) & 0xFF));
        }
    }

    // Escribir el buffer al archivo usando BinaryIO
    BinaryIO::writeBinaryFile(output + ".cppm", buffer);
}
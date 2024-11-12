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

void print_image_info(const ImageAos& img){
    std::cout << "Ancho: " << img.width << std::endl;
    std::cout << "Alto: " << img.height << std::endl;
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
    // Crear un archivo binario para escribir los datos comprimidos
    std::ofstream file(output + ".cppm", std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo " + output + ".cppm");
    }

    BinaryWriter writer(file);

    // Escribir un encabezado simple para el archivo comprimido
    writer.write_string("C6 ");
    writer.write_ascii_int(img.width);
    writer.write_string(" ");
    writer.write_ascii_int(img.height);
    writer.write_string(" ");
    writer.write_ascii_int(img.max_color_value);
    writer.write_string(" ");

    // Crear un mapa de frecuencia de colores y una lista de colores
    std::map<Pixel, int> freq_map;
    std::vector<Pixel> color_list;
    int color_index = 0;
    for (const auto& pixel : img.pixels) {
        if (freq_map.find(pixel) == freq_map.end()) {
            freq_map[pixel] = color_index++;
            color_list.push_back(pixel);
        }
    }

    // Escribir el número de entradas de la tabla de colores
    writer.write_ascii_int(static_cast<int>(color_list.size()));
    writer.write_string("\n");

    // Escribir la tabla de colores
    for (const auto& pixel : color_list) {
        if (img.max_color_value <= 255) {
            writer.write_uint8(static_cast<uint8_t>(pixel.r));
            writer.write_uint8(static_cast<uint8_t>(pixel.g));
            writer.write_uint8(static_cast<uint8_t>(pixel.b));
        } else {
            writer.write_uint16(pixel.r);
            writer.write_uint16(pixel.g);
            writer.write_uint16(pixel.b);
        }
    }

    // Escribir los índices de los colores
    int index_size = (color_list.size() < 256) ? 1 : (color_list.size() < 65536) ? 2 : 4;
    for (const auto& pixel : img.pixels) {
        int index = freq_map[pixel];
        for (int i = 0; i < index_size; ++i) {
            writer.write_uint8(static_cast<uint8_t>((index >> (i * 8)) & 0xFF));
        }
    }

    // Verificar si hubo un error al escribir los datos comprimidos
    if (!file) {
        throw std::runtime_error("Error: Error al escribir los datos de los píxeles comprimidos.");
    }
}
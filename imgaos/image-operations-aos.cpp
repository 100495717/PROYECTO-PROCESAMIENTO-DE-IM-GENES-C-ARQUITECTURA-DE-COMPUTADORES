#include "image-operations-aos.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <limits>

void print_image_info(const ImageAos& img){
    std::cout << "Ancho: " << img.width << std::endl;
    std::cout << "Alto: " << img.height << std::endl;
    std::cout << "Píxeles totales: " << img.pixels.size() << std::endl;

}

void max_level(ImageAos& img, int maxlevel){
    double scale = static_cast<double>(maxlevel) / img.max_color_value;
    // Verificar si el nivel máximo es válido
    if (maxlevel <= 0 || maxlevel > 65535) {
        throw std::invalid_argument("El nivel máximo debe ser mayor a cero y menor a 65535.");
    }
    // Verificar si necesitamos cambiar la representacion de los pixeles
    bool fromOneToTwoBytes = (img.max_color_value <= 255 && maxlevel > 255);
    bool fromTwoToOneBytes = (img.max_color_value > 255 && maxlevel <= 255);

    // Crear un nuevo vector de píxeles si es necesario cambiar la representación
    std::vector<Pixel> new_pixels;
    if (fromOneToTwoBytes || fromTwoToOneBytes){
        new_pixels.resize(img.width * img.height);
    }
    // Iterar sobre los píxeles de la imagen
    for (size_t i=0; i<img.pixels.size();++i){
        int originalRed = img.pixels[i].r;
        int originalGreen = img.pixels[i].g;
        int originalBlue = img.pixels[i].b;

        // Escalar los valores de los canales RGB con el nuevo valor máximo
        int newRed = static_cast<int>(std::round(originalRed * scale));
        int newGreen = static_cast<int>(std::round(originalGreen * scale));
        int newBlue = static_cast<int>(std::round(originalBlue * scale));

        if (fromOneToTwoBytes){
            // Convertir a 2 bytes por canal
            new_pixels[i].r = newRed & 0xFF;
            new_pixels[i].g = newGreen & 0xFF;
            new_pixels[i].b = newBlue & 0xFF;
        }
        else if (fromTwoToOneBytes){
            // Convertir a 1 byte por canal
            new_pixels[i].r = static_cast<unsigned char>(newRed);
            new_pixels[i].g = static_cast<unsigned char>(newGreen);
            new_pixels[i].b = static_cast<unsigned char>(newBlue);
        }
        else{
            // Mantener la representación original
            img.pixels[i].r = static_cast<unsigned char>(newRed);
            img.pixels[i].g = static_cast<unsigned char>(newGreen);
            img.pixels[i].b = static_cast<unsigned char>(newBlue);
        }
    }
    // Actualizar el nivel máximo de la imagen
    if (fromOneToTwoBytes || fromTwoToOneBytes){
        img.pixels = std::move(new_pixels);
    }
    img.max_color_value = maxlevel;

}

void resize_image(ImageAos& img, int width, int height){
    // Comprobamos si las dimensiones proporcionadas son válidas
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Las dimensiones de la imagen deben ser mayores a cero.");
    }
    // Crear un nuevo vector de píxeles con las dimensiones de la nueva imagen
    std::vector<Pixel> new_pixels(width * height);
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_pixels[y * width + x] = img.pixels[py * img.width + px];
            
        }
    }
    // Actualizar las dimensiones y los píxeles de la imagen
    img.width = width;
    img.height = height;
    img.pixels = std::move(new_pixels);
}

// Función auxiliar para calcular la distancia euclídea entre dos píxeles
double color_distance(const Pixel& a, const Pixel& b){
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

void cut_least_freq(ImageAos& img, int n) {
    // Paso 1: Determinar la frecuencia de cada color
    std::map<Pixel, int> freq_map;
    for (const auto& pixel : img.pixels) {
        freq_map[pixel]++;
    }
    // Comprobamos si 'n' es válido
    if (n <= 0) {
        throw std::invalid_argument("El número de colores a eliminar debe ser mayor que cero.");
    }
    if (n > freq_map.size()) {
        throw std::invalid_argument("El número de colores a eliminar no puede ser mayor que el número de colores únicos en la imagen.");
    }
    // Paso 2: Identificar los n colores menos frecuentes
    std::vector<std::pair<Pixel, int>> freq_vec(freq_map.begin(), freq_map.end());
    std::sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    // Crear un vector de los colores menos frecuentes
    std::vector<Pixel> least_freq_pixels;
    for (int i = 0; i < n && i < freq_vec.size(); ++i) {
        least_freq_pixels.push_back(freq_vec[i].first);
    }
    // Mensaje de depuración: Imprimir los colores menos frecuentes
    std::cout << "Colores menos frecuentes:" << std::endl;
    for (const auto& pixel : least_freq_pixels) {
        std::cout << "(" << static_cast<int>(pixel.r) << ", " << static_cast<int>(pixel.g) << ", " << static_cast<int>(pixel.b) << ")" << std::endl;
    }
    // Paso 3: Reemplazar los colores menos frecuentes por el color más cercano restante
    std::vector<Pixel> remaining_pixels;
    for (const auto& [pixel, _] : freq_map) {
        if (std::find(least_freq_pixels.begin(), least_freq_pixels.end(), pixel) == least_freq_pixels.end()) {
            remaining_pixels.push_back(pixel);
        }
    }
    for (auto& pixel : img.pixels) {
        if (std::find(least_freq_pixels.begin(), least_freq_pixels.end(), pixel) != least_freq_pixels.end()) {
            Pixel old_pixel = pixel;
            pixel = find_closest_pixel(pixel, remaining_pixels);
            // Mensaje de depuración: Imprimir el reemplazo de color
            std::cout << "Reemplazando color (" << static_cast<int>(old_pixel.r) << ", " << static_cast<int>(old_pixel.g) << ", " << static_cast<int>(old_pixel.b) << ") "
                      << "por (" << static_cast<int>(pixel.r) << ", " << static_cast<int>(pixel.g) << ", " << static_cast<int>(pixel.b) << ")" << std::endl;
        }
    }
    
}

void compress_image(const ImageAos& img, const std::string& output) {
    // Crear un archivo binario para escribir los datos comprimidos
    std::ofstream file(output, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo " + output);
    }
    // Escribir un encabezado simple para el archivo comprimido
    file << "C6\n" << img.width << " " << img.height << "\n";

    // Escribir los datos de los píxeles comprimidos, simplemente se copian los datos
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
    file << color_list.size() << "\n";
    // Escribir la tabla de colores
    for (const auto& pixel : color_list) {
        if (img.max_color_value <= 255){
            file.put(pixel.r);
            file.put(pixel.g);
            file.put(pixel.b);
        }
        else{
            file.put(pixel.r >> 8);
            file.put(pixel.r & 0xFF);
            file.put(pixel.g >> 8);
            file.put(pixel.g & 0xFF);
            file.put(pixel.b >> 8);
            file.put(pixel.b & 0xFF);
        }
    }
    // Escribir los índices de los colores
    int index;
    if (color_list.size() < 256){
        index = 1;
    } else if (color_list.size() < 65536){
        index = 2;
    } else {
        index = 4;
    }
    for (const auto& pixel : img.pixels){
        int indice = freq_map[pixel];
        for (int i = 0; i < index; i++){
            file.put((indice >> (i*8)) & 0xFF);
        }
    }
    // Verificar si hubo un error al escribir los datos comprimidos
    if (!file) {
        throw std::runtime_error("Error: Error al escribir los datos de los píxeles comprimidos.");
    }
}
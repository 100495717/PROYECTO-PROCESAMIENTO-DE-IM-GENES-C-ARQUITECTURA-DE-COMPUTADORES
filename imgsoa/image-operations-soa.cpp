#include "image-operations-soa.hpp"
#include "../common/binaryio.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <limits>
#include <future>
#include <execution>
#include <cmath>
#include <fstream>
#include <sstream>

void load_image_from_file_soa(const std::string& filePath, ImageSoa& img) {
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
    img.r.resize(static_cast<size_t>(img.width) * static_cast<size_t>(img.height));
    img.g.resize(static_cast<size_t>(img.width) * static_cast<size_t>(img.height));
    img.b.resize(static_cast<size_t>(img.width) * static_cast<size_t>(img.height));

    // Leer los píxeles de la imagen
    for (size_t i = 0; i < img.r.size(); ++i) {
        if (pixel_size == 3) {
            // Leer 1 byte por componente (RGB)
            img.r[i] = buffer[offset++];
            img.g[i] = buffer[offset++];
            img.b[i] = buffer[offset++];
        } else {
            // Leer 2 bytes por componente (RGB), en formato little-endian
            img.r[i] = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
            img.g[i] = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
            img.b[i] = buffer[offset] | (buffer[offset + 1] << 8);
            offset += 2;
        }
    }
}

void write_image_to_file_soa(const std::string& filePath, const ImageSoa& img) {
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
        for (size_t i = 0; i < img.r.size(); ++i) {
            buffer.push_back(static_cast<uint8_t>(img.r[i]));
            buffer.push_back(static_cast<uint8_t>(img.g[i]));
            buffer.push_back(static_cast<uint8_t>(img.b[i]));
        }
    } else {
        // Si el valor máximo de color es > 255, cada componente se guarda en 2 bytes (little-endian)
        for (size_t i = 0; i < img.r.size(); ++i) {
            buffer.push_back(static_cast<uint8_t>(img.r[i] & 0xFF));
            buffer.push_back(static_cast<uint8_t>((img.r[i] >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>(img.g[i] & 0xFF));
            buffer.push_back(static_cast<uint8_t>((img.g[i] >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>(img.b[i] & 0xFF));
            buffer.push_back(static_cast<uint8_t>((img.b[i] >> 8) & 0xFF));
        }
    }

    // Escribir el buffer al archivo usando BinaryIO
    BinaryIO::writeBinaryFile(filePath, buffer);
}

//FUNCION INFO
void print_image_info_soa(const ImageSoa& img) {
    std::cout << "Ancho: " << img.width << std::endl;
    std::cout << "Alto: " << img.height << std::endl;
    std::cout << "Valor máximo de color: " << img.max_color_value << std::endl;
}


void max_level(ImageSoa& img, int maxlevel) {
    // Comprobamos que el nuevo max level es correcto
    if (maxlevel <= 0 || maxlevel > 65535) {
        throw std::invalid_argument("El nivel máximo debe ser mayor a cero y menor o igual a 65535.");
    }

    // Calculamos el factor de escalado
    double scale = static_cast<double>(maxlevel) / img.max_color_value;
    std::cout << "Factor de escala calculado: " << scale << std::endl;

    // Calculamos el nuevo valor de cada pixel (RGB)
    for (size_t i = 0; i < img.r.size(); ++i) {
        // Para evitar desbordamientos, nos aseguramos de que los nuevos valores no excedan el limite(65535)
        int scaledRed = std::min(static_cast<int>(std::round(img.r[i] * scale)), 65535);
        int scaledGreen = std::min(static_cast<int>(std::round(img.g[i] * scale)), 65535);
        int scaledBlue = std::min(static_cast<int>(std::round(img.b[i] * scale)), 65535);

        // Utilizamos unsigned short para que podamos representar (0-65535)
        img.r[i] = static_cast<unsigned short>(scaledRed);
        img.g[i] = static_cast<unsigned short>(scaledGreen);
        img.b[i] = static_cast<unsigned short>(scaledBlue);
    }

    // Actualiza el valor máximo de color después del escalado
    img.max_color_value = maxlevel;
    std::cout << "Escalado completado con nuevo valor máximo: " << img.max_color_value << std::endl;
}

//Función que combina los cuatro pixeles vecinos de un pixel dadas sus coordenadas (x,y)
unsigned short interpolacionBilineal(double x, double y,
                                    unsigned short c00, unsigned short c10,
                                    unsigned short c01, unsigned short c11) {
    // Calculamos mediante la interpolacion c1, en el eje x entre c00 y c10 
    double c1 = c00 * (1 - x) + c10 * x;
    // Calculamos mediante la interpolacion c2, en el eje x entre c01 y c11
    double c2 = c01 * (1 - x) + c11 * x;
    // Devolvemos el valor final de la interpolacion en el eje y
    //Se suma 0.5 para redondear al entero mas cercano
    return static_cast<unsigned short>(c1 * (1 - y) + c2 * y + 0.5);
} 

void resize_image_soa(ImageSoa& img, int new_width, int new_height) {
    if (new_width <= 0 || new_height <= 0)
        throw std::invalid_argument("Dimensiones deben ser mayores a cero.");
    if (new_width == 1 && new_height == 1) {
        unsigned long sum_r = 0, sum_g = 0, sum_b = 0;
        for (std::size_t i = 0; i < img.r.size(); ++i) {
            sum_r += img.r[i];
            sum_g += img.g[i];
            sum_b += img.b[i];
        }
        img.r = { static_cast<unsigned short>(sum_r / img.r.size()) };
        img.g = { static_cast<unsigned short>(sum_g / img.g.size()) };
        img.b = { static_cast<unsigned short>(sum_b / img.b.size()) };
        img.width = img.height = 1;
        return;}

    // Código actual de redimensionamiento con interpolación bilineal
    std::vector<unsigned short> new_r(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));
    std::vector<unsigned short> new_g(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));
    std::vector<unsigned short> new_b(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));

    double x_ratio = static_cast<double>(img.width) / new_width;
    double y_ratio = static_cast<double>(img.height) / new_height;
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            double gx = x * x_ratio, gy = y * y_ratio;
            int xl = static_cast<int>(std::floor(gx)), xh = std::min(static_cast<int>(std::ceil(gx)), img.width - 1);
            int yl = static_cast<int>(std::floor(gy)), yh = std::min(static_cast<int>(std::ceil(gy)), img.height - 1);
            double dx = gx - xl, dy = gy - yl;
            auto idx = [&](int row, int col) { return static_cast<std::vector<unsigned short>::size_type>(row * img.width + col); };
            auto interp = [&](const std::vector<unsigned short>& ch) {
                return interpolacionBilineal(dx, dy, ch[idx(yl, xl)], ch[idx(yl, xh)], ch[idx(yh, xl)], ch[idx(yh, xh)]);
            };
            new_r[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interp(img.r);
            new_g[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interp(img.g);
            new_b[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interp(img.b);
        }
    }

    img.width = new_width;
    img.height = new_height;
    img.r = std::move(new_r);
    img.g = std::move(new_g);
    img.b = std::move(new_b);
}

void cutfreq(ImageSoa& imagen, int n) {
    std::unordered_map<Color, int, HashColor> frecuenciaColor;
    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};
        ++frecuenciaColor[color];}
    if (n <= 0 || n > static_cast<int>(frecuenciaColor.size())) {
        throw std::invalid_argument("Número de colores a eliminar debe estar entre 1 y el número de colores únicos en la imagen.");
    }

    std::vector<std::pair<Color, int>> vecFrecuenciaColor(frecuenciaColor.begin(), frecuenciaColor.end());
    std::nth_element(vecFrecuenciaColor.begin(), vecFrecuenciaColor.begin() + n, vecFrecuenciaColor.end(),
                     [](const auto& a, const auto& b) { return a.second < b.second; });
    vecFrecuenciaColor.resize(static_cast<std::vector<std::pair<std::array<unsigned short, 3>, int>>::size_type>(n));

    std::unordered_set<Color, HashColor> coloresMenosFrecuentes;
    for (const auto& color : vecFrecuenciaColor) {
        coloresMenosFrecuentes.insert(color.first);
    }

    std::vector<Color> coloresRestantes;
    for (const auto& color : frecuenciaColor) {
        if (!coloresMenosFrecuentes.count(color.first)) {
            coloresRestantes.push_back(color.first);
        }
    }

    if (coloresRestantes.empty()) {
        std::cerr << "No hay colores restantes para construir el KDTree. Abortando operación." << std::endl;
        return;
    }
    
    KDTree tree(coloresRestantes);
    std::unordered_map<Color, Color, HashColor> mapaReemplazo;
    // Buscamos el color más cercano al color que vamos a eliminar
    for (const auto& color : coloresMenosFrecuentes) {
        mapaReemplazo[color] = tree.nearestNeighbor(color);
    }

    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};
        if (coloresMenosFrecuentes.count(color)) {
            Color colorReemplazo = mapaReemplazo[color];
            imagen.r[i] = colorReemplazo[0];
            imagen.g[i] = colorReemplazo[1];
            imagen.b[i] = colorReemplazo[2];
            }
        }
    }


//COMPRESS
void compress_image_soa(const ImageSoa& img, const std::string& output) {
    std::vector<uint8_t> buffer;

    // Escribir el encabezado (por ejemplo, "C6 ancho alto max_color_value")
    std::string header = "C6 " + std::to_string(img.width) + " " + std::to_string(img.height) + " " + std::to_string(img.max_color_value) + " ";
    buffer.insert(buffer.end(), header.begin(), header.end());

    // Crear un mapa de frecuencia de colores y una lista de colores únicos
    std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> color_map;
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> color_list;
    int color_index = 0;
    for (size_t i = 0; i < img.r.size(); ++i) {
        auto color = std::make_tuple(img.r[i], img.g[i], img.b[i]);
        if (color_map.find(color) == color_map.end()) {
            color_map[color] = color_index++;
            color_list.push_back(color);
        }
    }

    // Escribir el número de colores únicos
    std::string color_count = std::to_string(color_list.size()) + "\n";
    buffer.insert(buffer.end(), color_count.begin(), color_count.end());

    // Escribir la tabla de colores
    for (const auto& color : color_list) {
        uint16_t r, g, b;
        std::tie(r, g, b) = color;
        if (img.max_color_value <= 255) {
            buffer.push_back(static_cast<uint8_t>(r));
            buffer.push_back(static_cast<uint8_t>(g));
            buffer.push_back(static_cast<uint8_t>(b));
        } else {
            buffer.push_back(static_cast<uint8_t>(r >> 8));
            buffer.push_back(static_cast<uint8_t>(r & 0xFF));
            buffer.push_back(static_cast<uint8_t>(g >> 8));
            buffer.push_back(static_cast<uint8_t>(g & 0xFF));
            buffer.push_back(static_cast<uint8_t>(b >> 8));
            buffer.push_back(static_cast<uint8_t>(b & 0xFF));
        }
    }

    // Determinar el tamaño del índice
    int index_size = (color_list.size() <= 256) ? 1 : 
                     (color_list.size() <= 65536) ? 2 : 
                     (color_list.size() <= 4294967296) ? 4 : 
                     throw std::runtime_error("Tabla de colores > 2^32 no soportada");

    // Escribir los índices de los colores
    for (size_t i = 0; i < img.r.size(); ++i) {
        int index = color_map[std::make_tuple(img.r[i], img.g[i], img.b[i])];
        for (int j = 0; j < index_size; ++j) {
            buffer.push_back(static_cast<uint8_t>((index >> (j * 8)) & 0xFF));
        }
    }

    // Escribir el buffer al archivo usando BinaryIO
    try {
        BinaryIO::writeBinaryFile(output + ".cppm", buffer);
    } catch (const std::exception& e) {
        std::cerr << "Error al escribir el archivo: " << e.what() << std::endl;
        throw;
    }
}
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

// Función para calcular la distancia euclidiana al cuadrado entre dos colores
inline int distanciaColorCuadrada(const Color& color1, const Color& color2) {
    int difRojo = color1[0] - color2[0];
    int difVerde = color1[1] - color2[1];
    int difAzul = color1[2] - color2[2];
    return difRojo * difRojo + difVerde * difVerde + difAzul * difAzul;
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
    std::ofstream file(output, std::ios::binary);
    if (!file) throw std::runtime_error("Error: No se pudo abrir el archivo " + output);
    
    BinaryWriter writer(file);
    writer.write_string("C6 ");
    writer.write_ascii_int(img.width); writer.write_string(" ");
    writer.write_ascii_int(img.height); writer.write_string(" ");
    writer.write_ascii_int(img.max_color_value); writer.write_string(" ");
    
    std::map<std::tuple<unsigned short, unsigned short, unsigned short>, int> color_map;
    std::vector<std::tuple<unsigned short, unsigned short, unsigned short>> color_list;
    int color_index = 0;
    for (std::vector<unsigned short>::size_type i = 0; i < static_cast<std::vector<unsigned short>::size_type>(img.width * img.height); ++i) {
        auto color = std::make_tuple(img.r[i], img.g[i], img.b[i]);
        if (color_map.find(color) == color_map.end()) {
            color_map[color] = color_index++;
            color_list.push_back(color);
        }
    }
    writer.write_ascii_int(static_cast<int>(color_list.size())); writer.write_string("\n");

    for (const auto& color : color_list) {
        unsigned short r, g, b;
        std::tie(r, g, b) = color;
        if (img.max_color_value <= 255) {
            writer.write_uint8(static_cast<uint8_t>(r));
            writer.write_uint8(static_cast<uint8_t>(g));
            writer.write_uint8(static_cast<uint8_t>(b));
        } else {
            writer.write_uint16(r); writer.write_uint16(g); writer.write_uint16(b);
        }
    }
    int tamaño_indice = (color_list.size() <= 256) ? 1 : 
                        (color_list.size() <= 65536) ? 2 : 
                        (color_list.size() <= 4294967296) ? 4 : 
                        throw std::runtime_error("Tabla de colores > 2^32 no soportada");
    for (std::vector<unsigned short>::size_type i = 0; i < static_cast<std::vector<unsigned short>::size_type>(img.width * img.height); ++i) {
        int indice_color = color_map[std::make_tuple(img.r[i], img.g[i], img.b[i])];
        if (tamaño_indice == 1) writer.write_uint8(static_cast<uint8_t>(indice_color));
        else if (tamaño_indice == 2) writer.write_uint16(static_cast<uint16_t>(indice_color));
        else if (tamaño_indice == 4) writer.write_uint32(static_cast<uint32_t>(indice_color));
    }
    if (!file) throw std::runtime_error("Error: Fallo al escribir en el fichero de salida.");
    file.close();
}

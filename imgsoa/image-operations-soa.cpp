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
//Funcion resize
void resize_image_soa(ImageSoa& img, int new_width, int new_height) {
    if (new_width <= 0 || new_height <= 0) {
        throw std::invalid_argument("Las dimensiones de la imagen deben ser mayores a cero.");
    }

    if (new_width == 1 && new_height == 1) {
        // Calcular el promedio de cada canal para reducir la imagen a un solo píxel
        unsigned long sum_r = 0, sum_g = 0, sum_b = 0;
        for (std::size_t i = 0; i < img.r.size(); ++i) {
            sum_r += img.r[i];
            sum_g += img.g[i];
            sum_b += img.b[i];
        }
        img.r = { static_cast<unsigned short>(sum_r / img.r.size()) };
        img.g = { static_cast<unsigned short>(sum_g / img.g.size()) };
        img.b = { static_cast<unsigned short>(sum_b / img.b.size()) };
        img.width = 1;
        img.height = 1;
        return;
    }

    // Código actual de redimensionamiento con interpolación bilineal
    std::vector<unsigned short> new_redChannel(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));
    std::vector<unsigned short> new_greenChannel(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));
    std::vector<unsigned short> new_blueChannel(static_cast<std::vector<unsigned short>::size_type>(new_width) * static_cast<std::vector<unsigned short>::size_type>(new_height));

    double x_ratio = static_cast<double>(img.width) / new_width;
    double y_ratio = static_cast<double>(img.height) / new_height;

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            double gx = x * x_ratio;
            double gy = y * y_ratio;

            int xl = static_cast<int>(std::floor(gx));
            int xh = std::min(static_cast<int>(std::ceil(gx)), img.width - 1);
            int yl = static_cast<int>(std::floor(gy));
            int yh = std::min(static_cast<int>(std::ceil(gy)), img.height - 1);

            double diferencia_x = gx - xl;
            double diferencia_y = gy - yl;

            std::vector<unsigned short>::size_type indice00 = static_cast<std::vector<unsigned short>::size_type>(yl) * static_cast<std::vector<unsigned short>::size_type>(img.width) + static_cast<std::vector<unsigned short>::size_type>(xl);
            std::vector<unsigned short>::size_type indice10 = static_cast<std::vector<unsigned short>::size_type>(yl) * static_cast<std::vector<unsigned short>::size_type>(img.width) + static_cast<std::vector<unsigned short>::size_type>(xh);
            std::vector<unsigned short>::size_type indice01 = static_cast<std::vector<unsigned short>::size_type>(yh) * static_cast<std::vector<unsigned short>::size_type>(img.width) + static_cast<std::vector<unsigned short>::size_type>(xl);
            std::vector<unsigned short>::size_type indice11 = static_cast<std::vector<unsigned short>::size_type>(yh) * static_cast<std::vector<unsigned short>::size_type>(img.width) + static_cast<std::vector<unsigned short>::size_type>(xh);

            new_redChannel[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.r[indice00], img.r[indice10],
                img.r[indice01], img.r[indice11]
            );

            new_greenChannel[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.g[indice00], img.g[indice10],
                img.g[indice01], img.g[indice11]
            );

            new_blueChannel[static_cast<std::vector<unsigned short>::size_type>(y) * static_cast<std::vector<unsigned short>::size_type>(new_width) + static_cast<std::vector<unsigned short>::size_type>(x)] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.b[indice00], img.b[indice10],
                img.b[indice01], img.b[indice11]
            );
        }
    }

    img.width = new_width;
    img.height = new_height;
    img.r = std::move(new_redChannel);
    img.g = std::move(new_greenChannel);
    img.b = std::move(new_blueChannel);
}


// CUTFREQ
// 1. Definimos un array donde almacenaremos los componentes RGB de cada pixel
using Color = std::array<unsigned short, 3>;

//2. Para poder acceder rapidamente a la frecuencia de cada color, creamos una estructura hash
//Esta estructura, nos permitira calcular un hash unico por Color
struct HashColor {
    std::size_t operator()(const Color& color) const {
        //Desplazamos y combinamos los bits RGB
        return (static_cast<std::size_t>(color[0]) << 16) |
               (static_cast<std::size_t>(color[1]) << 8) |
               static_cast<std::size_t>(color[2]);
    }
};

//3. Desarrollamos la función para calcular la distancia euclidiana al cuadrado entre dos colores (evitandonos calcular la raiz)
inline int distanciaColorCuadrada(const Color& color1, const Color& color2) {
    int difRojo = color1[0] - color2[0];
    int difVerde = color1[1] - color2[1];
    int difAzul = color1[2] - color2[2];
    return difRojo * difRojo + difVerde * difVerde + difAzul * difAzul;
}

//4.Para la busqueda de colores cercanos, hemos decidido crear un KD-Tree para optimizar la búsqueda
//Este arbol almacenará los colores restantes una vez identidicado los n menos frecuentes
struct KDTree {
    struct Node {
        Color color;
        Node* left;
        Node* right;
    };

    Node* root;

    KDTree(const std::vector<Color>& colors) {
        std::cout << "Construyendo KDTree con " << colors.size() << " colores." << std::endl;
        root = build(colors, 0);
    }

    Node* build(const std::vector<Color>& colors, int depth) {
        if (colors.empty()) return nullptr;
        int axis = depth % 3;
        auto sortedColors = colors;
        std::nth_element(sortedColors.begin(), sortedColors.begin() + static_cast<std::vector<Color>::difference_type>(sortedColors.size() / 2), sortedColors.end(),
                         [axis](const Color& a, const Color& b) {
                             return a[static_cast<std::array<unsigned short, 3>::size_type>(axis)] < b[static_cast<std::array<unsigned short, 3>::size_type>(axis)];
                         });
        auto median = static_cast<std::vector<Color>::size_type>(sortedColors.size() / 2);
        Node* node = new Node{sortedColors[median], nullptr, nullptr};
        node->left = build(std::vector<Color>(sortedColors.begin(), sortedColors.begin() + static_cast<std::vector<Color>::difference_type>(median)), depth + 1);
        node->right = build(std::vector<Color>(sortedColors.begin() + static_cast<std::vector<Color>::difference_type>(median) + 1, sortedColors.end()), depth + 1);
        return node;
    }

    Color nearestNeighbor(const Color& target, Node* node, int depth, Color best, int& bestDist) const {
        if (!node) return best;

        int axis = depth % 3;
        int dist = distanciaColorCuadrada(target, node->color);

        if (dist < bestDist) {
            bestDist = dist;
            best = node->color;
        }

        Node* next = (target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] < node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) ? node->left : node->right;
        Node* other = (next == node->left) ? node->right : node->left;

        best = nearestNeighbor(target, next, depth + 1, best, bestDist);

        if ((target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] - node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) * (target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] - node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) < bestDist) {
            best = nearestNeighbor(target, other, depth + 1, best, bestDist);
        }

        return best;
    }

    Color nearestNeighbor(const Color& target) const {
        if (!root) {
            std::cerr << "KDTree está vacío. No se puede encontrar el vecino más cercano." << std::endl;
            return target; // Devolver el color original si el árbol está vacío
        }
        int bestDist = std::numeric_limits<int>::max();
        return nearestNeighbor(target, root, 0, root->color, bestDist);
    }

    ~KDTree() {
        deleteTree(root);
    }

private:
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
};

// Función principal cutfreq
void cutfreq(ImageSoa& imagen, int n) {
    std::cout << "Iniciando cutfreq con n = " << n << std::endl;

    // Primero contamos la frecuencia de cada color, para ello usamos unordened_map
    std::unordered_map<Color, int, HashColor> frecuenciaColor;
    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};
        ++frecuenciaColor[color];
    }

    std::cout << "Frecuencia de colores calculada. Total de colores únicos: " << frecuenciaColor.size() << std::endl;

    // Comprobamos que n es un número entero positivo y que no sea mayor que el número de colores únicos que hay en la imagen
    if (n <= 0 || n > static_cast<int>(frecuenciaColor.size())) {
        throw std::invalid_argument("Número de colores a eliminar debe estar entre 1 y el número de colores únicos en la imagen.");
    }

    // Convertimos frecuenciaColor en un vector para poder ordenar los colores por sus frecuencias
    std::vector<std::pair<Color, int>> vecFrecuenciaColor(frecuenciaColor.begin(), frecuenciaColor.end());
    std::nth_element(vecFrecuenciaColor.begin(), vecFrecuenciaColor.begin() + n, vecFrecuenciaColor.end(),
                     [](const auto& a, const auto& b) { return a.second < b.second; });

    // Reducimos el vector para que solo tenga los n colores menos frecuentes
    vecFrecuenciaColor.resize(static_cast<std::vector<std::pair<std::array<unsigned short, 3>, int>>::size_type>(n));

    std::cout << "Colores menos frecuentes identificados. Total: " << vecFrecuenciaColor.size() << std::endl;

    // Esos colores menos frecuentes, los añadimos al conjunto coloresMenosFrecuentes
    std::unordered_set<Color, HashColor> coloresMenosFrecuentes;
    for (const auto& color : vecFrecuenciaColor) {
        coloresMenosFrecuentes.insert(color.first);
    }

    std::cout << "Colores menos frecuentes almacenados en el conjunto." << std::endl;

    // Creamos un vector con los colores restantes
    std::vector<Color> coloresRestantes;
    for (const auto& color : frecuenciaColor) {
        if (!coloresMenosFrecuentes.count(color.first)) {
            coloresRestantes.push_back(color.first);
        }
    }

    std::cout << "Colores restantes para construir el KDTree: " << coloresRestantes.size() << std::endl;

    // Verificamos si hay colores restantes para construir el KDTree
    if (coloresRestantes.empty()) {
        std::cerr << "No hay colores restantes para construir el KDTree. Abortando operación." << std::endl;
        return;
    }

    // Creamos el árbol con los colores que no vayan a ser eliminados de la imagen
    KDTree tree(coloresRestantes);
    std::unordered_map<Color, Color, HashColor> mapaReemplazo;

    // Recorremos todos los pixeles de la imagen para sustituir aquellos que hagan falta
    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};

        // Reemplazamos el color eliminado por su color más cercano en la imagen
        if (coloresMenosFrecuentes.count(color)) {
            Color colorReemplazo = mapaReemplazo[color];
            imagen.r[i] = colorReemplazo[0];
            imagen.g[i] = colorReemplazo[1];
            imagen.b[i] = colorReemplazo[2];
        }
    }

    std::cout << "Proceso de reemplazo completado." << std::endl;
}

//COMPRESS
void compress_image_soa(const ImageSoa& img, const std::string& output) {
    std::ofstream file(output + ".cppm", std::ios::binary);

    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo " + output);
    }


    BinaryWriter writer(file);
    writer.write_string("C6 ");
    writer.write_ascii_int(img.width);
    writer.write_string(" ");
    writer.write_ascii_int(img.height);
    writer.write_string(" ");
    writer.write_ascii_int(img.max_color_value);
    writer.write_string(" ");

    // Crear un mapa de colores únicos
    std::map<std::tuple<unsigned short, unsigned short, unsigned short>, int> color_map;
    std::vector<std::tuple<unsigned short, unsigned short, unsigned short>> color_list;
    int color_index = 0;

    for (int i = 0; i < img.width * img.height; ++i) {
        auto color = std::make_tuple(img.r[static_cast<std::vector<uint16_t>::size_type>(i)], 
        img.g[static_cast<std::vector<uint16_t>::size_type>(i)], 
        img.b[static_cast<std::vector<uint16_t>::size_type>(i)]);
        if (color_map.find(color) == color_map.end()) {
            color_map[color] = color_index++;
            color_list.push_back(color);
        }
    }

    int color_list_size = static_cast<int>(color_list.size());
    if (color_list_size < 0 || static_cast<size_t>(color_list_size) != color_list.size()) {
        throw std::overflow_error("color_list size exceeds int limits.");
    }
    writer.write_ascii_int(color_list_size);
    writer.write_string("\n");

    // Escribir la tabla de colores
    for (const auto& color : color_list) {
        unsigned short r, g, b;
        std::tie(r, g, b) = color;

        if (img.max_color_value <= 255) {
            writer.write_uint8(static_cast<unsigned char>(r));
            writer.write_uint8(static_cast<unsigned char>(g));
            writer.write_uint8(static_cast<unsigned char>(b));
        } else {
            writer.write_uint16(r);
            writer.write_uint16(g);
            writer.write_uint16(b);
        }
    }

    // Determinar el tamaño del índice
    int tamaño_indice = (color_list.size() <= 256) ? 1 : 
                        (color_list.size() <= 65536) ? 2 : 
                        (color_list.size() <= 4294967296) ? 4 : 
                        throw std::runtime_error("Error: No se soportan tablas de colores mayores a 2^32 colores");

    // Escribir los índices de color
    for (int i = 0; i < img.width * img.height; ++i) {
        auto color = std::make_tuple(img.r[static_cast<std::vector<uint16_t>::size_type>(i)],
         img.g[static_cast<std::vector<uint16_t>::size_type>(i)], 
         img.b[static_cast<std::vector<uint16_t>::size_type>(i)]);
        int indice_color = color_map[color];

        // Escribir el índice según el tamaño del índice
        if (tamaño_indice == 1) {
            writer.write_uint8(static_cast<uint8_t>(indice_color));
        } else if (tamaño_indice == 2) {
            writer.write_uint16(static_cast<uint16_t>(indice_color));
        } else if (tamaño_indice == 4) {
            writer.write_uint32(static_cast<uint32_t>(indice_color));
        }
    }

    if (!file) {
        throw std::runtime_error("Error: Fallo al escribir en el fichero de salida.");
    }

    file.close();
}
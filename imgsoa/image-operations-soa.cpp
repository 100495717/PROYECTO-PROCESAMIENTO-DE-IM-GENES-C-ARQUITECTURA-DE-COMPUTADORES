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
    std::cout << "Altp: " << img.height << std::endl;
    std::cout << "Valor máximo de color" << img.max_color_value << std::endl;
    std::cout << "Pixeles totales: " << img.width * img.height << std::endl;
}


void max_level(ImageSoa& img, int maxlevel) {
    // Comprobamos que el nuevo max level es correcto
    if (maxlevel <= 0 || maxlevel > 65535) {
        throw std::invalid_argument("El nivel máximo debe ser mayor a cero y menor o igual a 65535.");
    }

    //Si el nuevo max level coincide con el de la imagen, devolvemos la imagen tal cual
    if (img.max_color_value == maxlevel) {
        std::cout << "El valor máximo ya coincide con el valor deseado. No se necesita escalado." << std::endl;
        return;
    }

    // Calculamos el factor de escalado
    double scale = static_cast<double>(maxlevel) / img.max_color_value;
    std::cout << "Factor de escala calculado: " << scale << std::endl;

    // Calculamos el nuevo valor de cada pixel (RGB)
    for (size_t i = 0; i < img.r.size(); ++i) {
        // Para evitar desbordamientos, nos aseguramos de que los nuevos valores no excedan el limite(65535)
        int scaledRed = std::min(static_cast<int>(std::floor(img.r[i] * scale)), 65535);
        int scaledGreen = std::min(static_cast<int>(std::floor(img.g[i] * scale)), 65535);
        int scaledBlue = std::min(static_cast<int>(std::floor(img.b[i] * scale)), 65535);

        // Utilizamos unsigned short para que podamos representar (0-65535)
        img.r[i] = static_cast<unsigned short>(scaledRed);
        img.g[i] = static_cast<unsigned short>(scaledGreen);
        img.b[i] = static_cast<unsigned short>(scaledBlue);
    }

    // Actualiza el valor máximo de color después del escalado
    img.max_color_value = maxlevel;
    std::cout << "Escalado completado con nuevo valor máximo: " << img.max_color_value << std::endl;
}

void resize_image_soa(ImageSoa& img, int width, int height) {
    std::vector<uint16_t> new_r(static_cast<std::vector<uint16_t>::size_type>(width) * static_cast<std::vector<uint16_t>::size_type>(height));
    std::vector<uint16_t> new_g(static_cast<std::vector<uint16_t>::size_type>(width) * static_cast<std::vector<uint16_t>::size_type>(height));
    std::vector<uint16_t> new_b(static_cast<std::vector<uint16_t>::size_type>(width) * static_cast<std::vector<uint16_t>::size_type>(height));
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_r[static_cast<std::vector<uint16_t>::size_type>(y) * static_cast<std::vector<uint16_t>::size_type>(width) + static_cast<std::vector<uint16_t>::size_type>(x)] = img.r[static_cast<std::vector<uint16_t>::size_type>(py) * static_cast<std::vector<uint16_t>::size_type>(img.width) + static_cast<std::vector<uint16_t>::size_type>(px)];
            new_g[static_cast<std::vector<uint16_t>::size_type>(y) * static_cast<std::vector<uint16_t>::size_type>(width) + static_cast<std::vector<uint16_t>::size_type>(x)] = img.g[static_cast<std::vector<uint16_t>::size_type>(py) * static_cast<std::vector<uint16_t>::size_type>(img.width) + static_cast<std::vector<uint16_t>::size_type>(px)];
            new_b[static_cast<std::vector<uint16_t>::size_type>(y) * static_cast<std::vector<uint16_t>::size_type>(width) + static_cast<std::vector<uint16_t>::size_type>(x)] = img.b[static_cast<std::vector<uint16_t>::size_type>(py) * static_cast<std::vector<uint16_t>::size_type>(img.width) + static_cast<std::vector<uint16_t>::size_type>(px)];
        }
    }
    img.width = width;
    img.height = height;
    img.r = std::move(new_r);
    img.g = std::move(new_g);
    img.b = std::move(new_b);
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
    //Cada nodo del árbol almacrna un color y apunta a sus subárboles
    struct Node {
        Color color;
        Node* left;
        Node* right;
    };

    //Nodo raiz
   Node* root;

// Construimos el árbol dado un vector de colores
KDTree(const std::vector<Color>& colors) {
    root = build(colors, 0);
}

// Creamos una función recursiva en profundidad para construir el KDTree
Node* build(const std::vector<Color>& colors, int depth) {
    if (colors.empty()) return nullptr; // Si el árbol está vacío, devolvemos nullptr
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

    // Méto do que encuentra el color más cercano a target. Busqueda recursiva
    Color nearestNeighbor(const Color& target, Node* node, int depth, Color best, int& bestDist) const {
        //Caso base: si llegamos a un nodo nullptr, devolvemos el color mas cercano hasta ahora
        if (!node) return best;

        int axis = depth % 3;
        //Calculamos la distancia entrre el nodo correspondiente target y el color que corresponda
        int dist = distanciaColorCuadrada(target, node->color);

        //Si la distancia es menor que la mejor encontrada, actualizamos best node
        if (dist < bestDist) {
            bestDist = dist;
            best = node->color;
        }

        //Determinamos next, que es el subárbol que nos conviene explorar
        Node* next = (target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] < node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) ? node->left : node->right;
        //En caso de tener que seguir explorando, se buscará por el subárbol other
        Node* other = (next == node->left) ? node->right : node->left;

        //Llamamos de nuevo al méto do recursivamente (busqueda en profundidad)
        best = nearestNeighbor(target, next, depth + 1, best, bestDist);

        //En caso de que podamos encontrar un mejor color, buscamos por el subarbol other
        if ((target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] - node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) * (target[static_cast<std::array<uint16_t, 3>::size_type>(axis)] - node->color[static_cast<std::array<uint16_t, 3>::size_type>(axis)]) < bestDist) {
            best = nearestNeighbor(target, other, depth + 1, best, bestDist);
        }

        //Devolvemos el mejor color que hayamos encontrado
        return best;
    }

    //Méto do público que busca el vecino mas cercano llamando al méto do anterior
    Color nearestNeighbor(const Color& target) const {
        int bestDist = std::numeric_limits<int>::max();
        return nearestNeighbor(target, root, 0, root->color, bestDist);
    }

    //Liberamos la memoria que ocupaba el árbol
    ~KDTree() { deleteTree(root); }

private:
    //Méto do que destruye los nodos del árbol creado
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
};

// Función principal cutfreq
void cutfreq(ImageSoa& imagen, int n) {
    //Primero contamos la frecuencia de cada color, para ello usamos unordened_map
    //Unordened_map nos permite ubicar rapidamente cada color y ver su frecuencia con la estructura Hash creada antes
    std::unordered_map<Color, int, HashColor> frecuenciaColor;
    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};
        ++frecuenciaColor[color];
    }

    //Comprobamos que n es un número entero positivo y que no sea mayor que el número de colores únicos que hay en la imagen
    if (n <= 0 || n > static_cast<int>(frecuenciaColor.size())) {
        throw std::invalid_argument("Número de colores a eliminar debe estar entre 1 y el número de colores únicos en la imagen.");
    }

    //Convertimos frecuenciaColor en un vector para poder ordenar los colores por sus frecuencias
    std::vector<std::pair<Color, int>> vecFrecuenciaColor(frecuenciaColor.begin(), frecuenciaColor.end());
    std::nth_element(vecFrecuenciaColor.begin(), vecFrecuenciaColor.begin() + n, vecFrecuenciaColor.end(),
                     [](const auto& a, const auto& b) { return a.second < b.second; });

    //Reducimos el vector para que solo tenga los n colores menos frecuentes
    vecFrecuenciaColor.resize(static_cast<std::vector<std::pair<std::array<unsigned short, 3>, int>>::size_type>(n));

    //Esos colores menos frecuentes, los añadimos al conjunto coloresMenosFrecuentes
    std::unordered_set<Color, HashColor> coloresMenosFrecuentes;
    for (const auto& color : vecFrecuenciaColor) {
        coloresMenosFrecuentes.insert(color.first);
    }

    //Creamos un vector con los colores restantes
    std::vector<Color> coloresRestantes;
    for (const auto& color : frecuenciaColor) {
        if (!coloresMenosFrecuentes.count(color.first)) {
            coloresRestantes.push_back(color.first);
        }
    }

    //Creamos el árbol con los colores que no vayan a ser eliminados de la imagen
    KDTree tree(coloresRestantes);
    std::unordered_map<Color, Color, HashColor> mapaReemplazo;

    //Buscamos el color más cercano al color que vamos a eliminar
    for (const auto& color : coloresMenosFrecuentes) {
        mapaReemplazo[color] = tree.nearestNeighbor(color);
    }

    //Recorremos  todos los pixeles de la imagen para sustituir aquellos que hagan falta
    for (size_t i = 0; i < imagen.r.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.r[i]),
                       static_cast<unsigned short>(imagen.g[i]),
                       static_cast<unsigned short>(imagen.b[i])};

        //Reemplazamos el color eliminado por su color más cercano en la imagen

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
    // Creamos el archivo donde vamos a escribir la imagen en formato CPPM
    std::ofstream file(output, std::ios::binary);

    // Comprobamos si hay algun error a la hora de abrir el fichero de escritura
    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo " + output);
    }

    // Escribimos el número mágico para archivos en formatoCPPM (con un espacio en blanco)
    BinaryWriter writer(file);
    // Escribir un encabezado simple para el archivo comprimido
    writer.write_string("C6 ");
    writer.write_ascii_int(img.width);
    writer.write_string(" ");
    writer.write_ascii_int(img.height);
    writer.write_string(" ");
    writer.write_ascii_int(img.max_color_value);
    writer.write_string(" ");

    // Creamos un mapa para cada color, y asi poder contar cuantos colores únicos hay en la imagen de entrada
    std::map<std::tuple<unsigned short, unsigned short, unsigned short>, int> color_map;
    std::vector<std::tuple<unsigned short, unsigned short, unsigned short>> color_list;

    // Cada color tendra un índice en la tabla de colores
    int color_index = 0;

    // Para cada pixel en la imagen:
    for (int i = 0; i < img.width * img.height; ++i) {
        // Obtenemos el color de ese pixel combinando los valores RGB
        auto color = std::make_tuple(img.r[static_cast<std::vector<unsigned short>::size_type>(i)],
                             img.g[static_cast<std::vector<unsigned short>::size_type>(i)],
                             img.b[static_cast<std::vector<unsigned short>::size_type>(i)]);

        // Agregamos el color al mapa de colores si no se habia añadido antes
        if (color_map.find(color) == color_map.end()) {
            color_map[color] = color_index++;
            color_list.push_back(color);
        }
    }

    // Escribimos cuantas entradas hay en la tabla de colores (seguido de un salto de linea)
    writer.write_string("\n");

    // Escribimos dicha tabla de colores
    for (const auto& color : color_list) {
        unsigned short r, g, b;
        std::tie(r, g, b) = color;

        // Si maxval <= 255 entonces usamos 1 byte por canal
        if (img.max_color_value <= 255) {
            writer.write_uint8(static_cast<unsigned char>(r));
            writer.write_uint8(static_cast<unsigned char>(g));
            writer.write_uint8(static_cast<unsigned char>(b));
        } else { // En caso contrario, utilizamos 2 bytes por canal
            writer.write_uint16(static_cast<unsigned char>(r));
            writer.write_uint16(static_cast<unsigned char>(g));
            writer.write_uint16(static_cast<unsigned char>(b));
        }
    }

    // A continuación, determinamos el tamaño del índice según las indicaciones del enunciado
    int tamaño_indice;

    //Si el tamaño de la lista de colores es 256 o menos (2^8), usamos 1 byte
    if (color_list.size() <= 256) {
        tamaño_indice = 1;


    //Si el tamaño de la lista de colores es 65536 o menos (2^16), usamos 2 bytes
    } else if (color_list.size() <= 65536) {
        tamaño_indice = 2;


    //Si el tamaño de la lista de colores es (2^32) o menos, usamos 4 bytes
    } else if (color_list.size() <= 4294967296) {
        tamaño_indice = 4;


    //Si el tamaño de la lista de colores es mayor a 2^32, lanzamos una error
    } else {
        throw std::runtime_error("Error: No se soportan tablas de colores mayores a 2^32 colores");
    }

    // Por último, escribimos los indices de color para cada pixel
    for (int i = 0; i < img.width * img.height; ++i) {
        //Obtenemos el color del pixel [i] y su indice en el mapa de colores
        auto color = std::make_tuple(img.r[static_cast<std::vector<unsigned short>::size_type>(i)],
                             img.g[static_cast<std::vector<unsigned short>::size_type>(i)],
                             img.b[static_cast<std::vector<unsigned short>::size_type>(i)]);
        int indice_color = color_map[color];

        //Escribimos el índice en el fichero
        for (int j = 0; j < tamaño_indice; ++j) {
            writer.write_uint8(static_cast<uint8_t>((indice_color >> (i * 8)) & 0xFF));
        }
    }

    // Si hay algun error a la hora de escribir, lanzamos un error
    if (!file) {
        throw std::runtime_error("Error: Fallo al escribir en el fichero de salida.");
    }

    // Cerramos el file
    file.close();
}
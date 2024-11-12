#include "imagesoa.hpp"
#include "image-operations-soa.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordened_map>
#include <unordened_set>
#include <tuple>
#include <limits>
#include <future>
#include <execution>



//FUNCION INFO
void print_image_info_soa(const ImageSoa& img) {
    std::cout << "Image width: " << img.width << std::endl;
    std::cout << "Image height: " << img.height << std::endl;
    std::cout << "Valor máximo de color" << img.max_color_value << std::endl;
    std::cout << "Total pixels: " << img.width * img.height << std::endl;
}


void max_level(ImageSoa& img, int maxlevel) {
    //Comprobamos que el nuevo max level es correcto
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
    for (size_t i = 0; i < img.redChannel.size(); ++i) {
        if (i < 10) {
            std::cout << "[Antes del escalado] Pixel " << i
                      << " - R: " << img.redChannel[i]
                      << ", G: " << img.greenChannel[i]
                      << ", B: " << img.blueChannel[i] << std::endl;
        }

        //Para evitar desbordamientos, nos aseguramos de que los nuevos valores no excedan el limite(65535)
        int scaledRed = std::min(static_cast<int>(std::floor(img.redChannel[i] * scale)), 65535);
        int scaledGreen = std::min(static_cast<int>(std::floor(img.greenChannel[i] * scale)), 65535);
        int scaledBlue = std::min(static_cast<int>(std::floor(img.blueChannel[i] * scale)), 65535);

        //Utilizamos unsigned short para que podamos representar (0-65535)
        img.redChannel[i] = static_cast<unsigned short>(scaledRed);
        img.greenChannel[i] = static_cast<unsigned short>(scaledGreen);
        img.blueChannel[i] = static_cast<unsigned short>(scaledBlue);

        //Estos prints son de depuracion, hay que quitarlos
        if (i < 10) {
            std::cout << "[Después del escalado] Pixel " << i
                      << " - R: " << img.redChannel[i]
                      << ", G: " << img.greenChannel[i]
                      << ", B: " << img.blueChannel[i] << std::endl;
        }
    }

    // Actualizamos el valor máximo de color después del escalado
    img.max_color_value = maxlevel;
    std::cout << "Escalado completado con nuevo valor máximo: " << img.max_color_value << std::endl;
}



//RESIZE
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
void resize_image(ImageSoA& img, int new_width, int new_height) {
    //Comprobamos que las dimensiones introducidas son correcctas
    if (new_width <= 0 || new_height <= 0) {
        throw std::invalid_argument("Las dimensiones de la imagen deben ser mayores a cero.");
    }

    // Creamos nuevos canales de color para la nueva imagen redimensionada
    std::vector<unsigned short> new_redChannel(new_width * new_height);
    std::vector<unsigned short> new_greenChannel(new_width * new_height);
    std::vector<unsigned short> new_blueChannel(new_width * new_height);

    //Calculamos la relacion entre w/w` y entre h/h`
    double x_ratio = static_cast<double>(img.width) / new_width;
    double y_ratio = static_cast<double>(img.height) / new_height;

    //Para cada posición de la nueva imagen:
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // Calculamos las coordenadas: x= x*x_ratio ;  y=y*y_ratio
            double gx = x * x_ratio;
            double gy = y * y_ratio;

            // Determinamos los 4 pixeles más próximos en la imagen original:
            int xl = static_cast<int>(std::floor(gx));
            int xh = std::min(static_cast<int>(std::ceil(gx)), img.width - 1);
            int yl = static_cast<int>(std::floor(gy));
            int yh = std::min(static_cast<int>(std::ceil(gy)), img.height - 1);

            // Calculamos la diferencias correspondientes desde la posición de la imagen original
            double diferencia_x = gx - xl;
            double diferencia_y = gy - yl;

            // Calculamos los nuevos índices de los pixeles adyacentes
            int indice00 = yl * img.width + xl;
            int indice10 = yl * img.width + xh;
            int indice01 = yh * img.width + xl;
            int indice11 = yh * img.width + xh;

            // Aplicamos la interpolacion bilineal en todos los arrays del struct ImageSoA (redChannel,blueChannel,greenChannel)
            new_redChannel[y * new_width + x] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.redChannel[indice00], img.redChannel[indice10],
                img.redChannel[indice01], img.redChannel[indice11]
            );

            new_greenChannel[y * new_width + x] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.greenChannel[indice00], img.greenChannel[indice10],
                img.greenChannel[indice01], img.greenChannel[indice11]
            );

            new_blueChannel[y * new_width + x] = interpolacionBilineal(
                diferencia_x, diferencia_y,
                img.blueChannel[indice00], img.blueChannel[indice10],
                img.blueChannel[indice01], img.blueChannel[indice11]
            );
        }
    }

    // Actualizamos las dimensiones
    img.width = new_width;
    img.height = new_height;

    // Actualizamos los arrays de los colores de la imagen
    img.redChannel = std::move(new_redChannel);
    img.greenChannel = std::move(new_greenChannel);
    img.blueChannel = std::move(new_blueChannel);
}

//CUTFREQ
//1. Definimos un array donde almacenaremos los componentes RGB de cada pixel
using Color = std::array<unsigned short, 3>;

//2. Para poder acceder rapidamente a la frecuencia de cada color, creamos una estructura hash
//Esta estructura, nos permitira calcular un hash unico por Color
struct HashColor {
    std::size_t operator()(const Color& color) const {
        //Desplazamos y combinamos los bits RGB 
        return (color[0] << 16) | (color[1] << 8) | color[2];
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

    //Construimos el árbol dado un vector de colores
    KDTree(const std::vector<Color>& colors) {
        root = build(colors, 0);
    }

    //Creamos una funcion recursiva en profundidad para construir el KDTree
    Node* build(const std::vector<Color>& colors, int depth) {
        if (colors.empty()) return nullptr; //Si el arbol esta vacio, devolvemos nullptr

        int axis = depth % 3;
        auto sortedColors = colors;
        std::nth_element(sortedColors.begin(), sortedColors.begin() + sortedColors.size() / 2, sortedColors.end(),
                         [axis](const Color& a, const Color& b) {
                             return a[axis] < b[axis];
                         });

        int median = sortedColors.size() / 2;
        Node* node = new Node{sortedColors[median], nullptr, nullptr};
        node->left = build(std::vector<Color>(sortedColors.begin(), sortedColors.begin() + median), depth + 1);
        node->right = build(std::vector<Color>(sortedColors.begin() + median + 1, sortedColors.end()), depth + 1);

        return node;
    }

    // Método que encuentra el color más cercano a target. Busqueda recursiva
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
        Node* next = (target[axis] < node->color[axis]) ? node->left : node->right;

        //En caso de tener que seguir explorando, se buscará por el subárbol other
        Node* other = (next == node->left) ? node->right : node->left;

        //Llamamos de nuevo al método recursivamente (busqueda en profundidad)
        best = nearestNeighbor(target, next, depth + 1, best, bestDist);

        //En caso de que podamos encontrar un mejor color, buscamos por el subarbol other
        if ((target[axis] - node->color[axis]) * (target[axis] - node->color[axis]) < bestDist) {
            best = nearestNeighbor(target, other, depth + 1, best, bestDist);
        }

        //Devolvemos el mejor color que hayamos encontrado
        return best;
    }

    //Método público que busca el vecino mas cercano llamando al método anterior
    Color nearestNeighbor(const Color& target) const {
        int bestDist = std::numeric_limits<int>::max();
        return nearestNeighbor(target, root, 0, root->color, bestDist);
    }

    //Liberamos la memoria que ocupaba el árbol
    ~KDTree() { deleteTree(root); }

private:
    //Método que destruye los nodos del árbol creado
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
};

// Función principal cutfreq
void cutfreq(ImageSoA& imagen, int n) {
    //Primero contamos la frecuencia de cada color, para ello usamos unordened_map
    //Unordened_map nos permite ubicar rapidamente cada color y ver su frecuencia con la estructura Hash creada antes
    std::unordered_map<Color, int, HashColor> frecuenciaColor;
    for (size_t i = 0; i < imagen.redChannel.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.redChannel[i]),
                       static_cast<unsigned short>(imagen.greenChannel[i]),
                       static_cast<unsigned short>(imagen.blueChannel[i])};
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
    vecFrecuenciaColor.resize(n);

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
    for (size_t i = 0; i < imagen.redChannel.size(); ++i) {
        Color color = {static_cast<unsigned short>(imagen.redChannel[i]),
                       static_cast<unsigned short>(imagen.greenChannel[i]),
                       static_cast<unsigned short>(imagen.blueChannel[i])};

        //Reemplazamos el color eliminado por su color más cercano en la imagen

        if (coloresMenosFrecuentes.count(color)) {
            Color colorReemplazo = mapaReemplazo[color];
            imagen.redChannel[i] = colorReemplazo[0];
            imagen.greenChannel[i] = colorReemplazo[1];
            imagen.blueChannel[i] = colorReemplazo[2];
        }
    }
}



void compress_image_soa(const ImageSoa& img, const std::string& output) {
    std::ofstream file(output, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }

    file << "CPPM\n" << img.width << " " << img.height << "\n";

    for (int i = 0; i < img.width * img.height; ++i) {
        file.put(img.r[i]);
        file.put(img.g[i]);
        file.put(img.b[i]);
    }

    if (!file) {
        throw std::runtime_error("Error al escribir los datos de los píxeles");
    }
}

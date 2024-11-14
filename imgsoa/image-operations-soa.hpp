#ifndef IMAGE_OPERATIONS_HPP
#define IMAGE_OPERATIONS_HPP

#include <array>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <limits>

class ImageSoa {
public:
    int width;
    int height;
    int max_color_value;
    std::vector<uint16_t> r; // Vector para los componentes rojos
    std::vector<uint16_t> g; // Vector para los componentes verdes
    std::vector<uint16_t> b; // Vector para los componentes azules

    // Constructor por defecto
    ImageSoa() : width(0), height(0), max_color_value(255) {}

    // Constructor con parámetros
    ImageSoa(int w, int h, int max_val) : width(w), height(h), max_color_value(max_val) {
        r.resize(static_cast<std::vector<uint16_t>::size_type>(w) * static_cast<std::vector<uint16_t>::size_type>(h));
        g.resize(static_cast<std::vector<uint16_t>::size_type>(w) * static_cast<std::vector<uint16_t>::size_type>(h));
        b.resize(static_cast<std::vector<uint16_t>::size_type>(w) * static_cast<std::vector<uint16_t>::size_type>(h));
    }

    // Constructor que acepta una ruta de archivo
    explicit ImageSoa(const std::string& filename);
};
// Definir un array para representar el color
using Color = std::array<uint16_t, 3>;

// Definir un hash personalizado para Color
struct ColorHash {
    std::size_t operator()(const Color& color) const {
        auto h1 = std::hash<uint16_t>{}(color[0]);
        auto h2 = std::hash<uint16_t>{}(color[1]);
        auto h3 = std::hash<uint16_t>{}(color[2]);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

// Estructura hash para Color
struct HashColor {
    std::size_t operator()(const Color& color) const {
        return (static_cast<std::size_t>(color[0]) << 16) |
               (static_cast<std::size_t>(color[1]) << 8) |
               static_cast<std::size_t>(color[2]);
    }
};

// Función para calcular la distancia euclidiana al cuadrado entre dos colores
inline int distanciaColorCuadrada(const Color& color1, const Color& color2) {
    int difRojo = color1[0] - color2[0];
    int difVerde = color1[1] - color2[1];
    int difAzul = color1[2] - color2[2];
    return difRojo * difRojo + difVerde * difVerde + difAzul * difAzul;
}

// Definición de la estructura KDTree
struct KDTree {
    struct Node {
        Color color;
        Node* left;
        Node* right;
    };

    Node* root;

    KDTree(const std::vector<Color>& colors) {
        root = build(colors, 0);
    }

    Node* build(const std::vector<Color>& colors, int depth) {
        if (colors.empty()) return nullptr;
        int axis = depth % 3;
        auto sortedColors = colors;
        std::nth_element(sortedColors.begin(), sortedColors.begin() + static_cast<std::vector<Color>::difference_type>(sortedColors.size() / 2), sortedColors.end(),
                         [axis](const Color& a, const Color& b) {
                             return a[static_cast<std::array<uint16_t, 3>::size_type>(axis)] < b[static_cast<std::array<uint16_t, 3>::size_type>(axis)];
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


void load_image_from_file_soa(const std::string& filename, ImageSoa& img);
void write_image_to_file_soa(const std::string& filename, const ImageSoa& img);
void print_image_info_soa(const ImageSoa& img);
void max_level(ImageSoa& img, int maxlevel);
void resize_image_soa(ImageSoa& img, int width, int height);
void cutfreq(ImageSoa& img, int n);
void compress_image_soa(const ImageSoa& img, const std::string& filename);

#endif // IMAGE_OPERATIONS_HPP
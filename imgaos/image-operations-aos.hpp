// image-operation-aos.hpp
#ifndef IMAGE_OPERATION_AOS_HPP
#define IMAGE_OPERATION_AOS_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <limits>
#include <unordered_map>
#include <iostream>
#include "../common/binaryio.hpp"

struct Pixel {
    uint16_t r;
    uint16_t g;
    uint16_t b;

    bool operator<(const Pixel& other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }

    bool operator==(const Pixel& other) const {
        return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
    }
};

namespace std {
    template <>
    struct hash<Pixel> {
        std::size_t operator()(const Pixel& pixel) const {
            return ((std::hash<uint16_t>()(pixel.r) ^ (std::hash<uint16_t>()(pixel.g) << 1)) >> 1) ^ (std::hash<uint16_t>()(pixel.b) << 1);
        }
    };
}

struct ImageAos {
    int width;
    int height;
    int max_color_value;
    std::vector<Pixel> pixels;

    ImageAos() : width(0), height(0), max_color_value(255) {}
};

// Función auxiliar para calcular la distancia de color (cuadrada) entre dos píxeles
inline int color_distance_squared(const Pixel& a, const Pixel& b) {
    int dr = a.r - b.r;
    int dg = a.g - b.g;
    int db = a.b - b.b;
    return dr * dr + dg * dg + db * db;
}


// Estructura para el KDTree

// Estructura para el KDTree
struct KDTree {
    struct Node {
        Pixel color;
        Node* left;
        Node* right;
    };

    Node* root;

    KDTree(const std::vector<Pixel>& colors) {
        root = build(colors, 0);
    }

    Node* build(const std::vector<Pixel>& colors, int depth) {
        if (colors.empty()) return nullptr;
        int axis = depth % 3;
        auto sortedColors = colors;
        std::nth_element(sortedColors.begin(), sortedColors.begin() + static_cast<std::vector<Pixel>::difference_type>(sortedColors.size()) / 2, sortedColors.end(),
                         [axis](const Pixel& a, const Pixel& b) {
                             return (axis == 0 ? a.r : (axis == 1 ? a.g : a.b)) <
                                    (axis == 0 ? b.r : (axis == 1 ? b.g : b.b));
                         });
        auto median = sortedColors.size() / 2;
        Node* node = new Node{sortedColors[median], nullptr, nullptr};
        node->left = build(std::vector<Pixel>(sortedColors.begin(), sortedColors.begin() + static_cast<std::vector<Pixel>::difference_type>(median)), depth + 1);
        node->right = build(std::vector<Pixel>(sortedColors.begin() + static_cast<std::vector<Pixel>::difference_type>(median) + 1, sortedColors.end()), depth + 1);
        return node;
    }

    Pixel nearestNeighbor(const Pixel& target, Node* node, int depth, Pixel best, int& bestDist) const {
        if (!node) return best;

        int axis = depth % 3;
        int dist = color_distance_squared(target, node->color);

        if (dist < bestDist) {
            bestDist = dist;
            best = node->color;
        }

        Node* next = (axis == 0 ? target.r < node->color.r :
                      axis == 1 ? target.g < node->color.g : target.b < node->color.b) ? node->left : node->right;
        Node* other = (next == node->left) ? node->right : node->left;

        best = nearestNeighbor(target, next, depth + 1, best, bestDist);

        if ((axis == 0 ? target.r - node->color.r :
             axis == 1 ? target.g - node->color.g : target.b - node->color.b) *
            (axis == 0 ? target.r - node->color.r :
             axis == 1 ? target.g - node->color.g : target.b - node->color.b) < bestDist) {
            best = nearestNeighbor(target, other, depth + 1, best, bestDist);
        }

        return best;
    }

    Pixel nearestNeighbor(const Pixel& target) const {
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



// Declaración de funciones
void load_image_from_file(const std::string& filePath, ImageAos& img);
void write_image_to_file(const std::string& filePath, const ImageAos& img);
void print_image_info(const ImageAos& img);
template <typename T>
T clamp(T value, T min, T max);
void max_level(ImageAos& img, int maxlevel);
void print_pixels(const std::vector<Pixel>& pixels);
void resize_image(ImageAos& img, int width, int height);
double color_distance(const Pixel& a, const Pixel& b);
Pixel find_closest_pixel(const Pixel& target, const std::vector<Pixel>& pixels);
void cut_freq(ImageAos& img, int n);
void compress_image(const ImageAos& img, const std::string& output);

#endif 
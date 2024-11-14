#ifndef IMAGE_OPERATIONS_HPP
#define IMAGE_OPERATIONS_HPP

#include "ppm-soa.hpp"
#include <array>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <limits>

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

void print_image_info_soa(const ImageSoa& img);
void max_level(ImageSoa& img, int maxlevel);
void resize_image_soa(ImageSoa& img, int width, int height);
void cutfreq(ImageSoa& img, int n);
void compress_image_soa(const ImageSoa& img, const std::string& filename);

#endif // IMAGE_OPERATIONS_HPP

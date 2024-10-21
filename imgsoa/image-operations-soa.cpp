#include "imagesoa.hpp"
#include "image-operations-soa.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>


//FUNCION INFO
void print_image_info_soa(const ImageSoa& img) {
    std::cout << "Image width: " << img.width << std::endl;
    std::cout << "Image height: " << img.height << std::endl;
    std::cout << "Valor máximo de color" << img.max_color_value << std::endl;
    std::cout << "Total pixels: " << img.width * img.height << std::endl;
}


//FUNCION MAXLEVEL: ESCALADO DE INTENSIDAD
void max_level_soa(ImageSoa& img, int maxlevel) {
    //Calculamos el factor de escalado como un valor de coma flotante
    float scalingFactor = float(maxLevel) / img.max_color_value;

    //Determinamos si tenemos que cambiar la representación de los pixeles
    bool fromOneToTwoBytes = (img.max_color_value<256 && maxlevel>=256);
    bool fromTwoToOneByte = (img.max_color_value>=256 && maxlevel<256);

    //Creamos vectores para almacenar los nuevos valores RGB escalados
    std::vector<unsigned char> newRedChannel;
    std::vector<unsigned char> newGreenChannel;
    std:vector<unsigned char> newBlueChannel;
    
     //Recorremos todos los píxeles de la imagen original
    for (int i = 0; i < image.width * image.height; ++i) {
        int originalRed, originalGreen, originalBlue;

        // Si la imagen original estaba en formato de 2 bytes por canal (maxColorValue >= 256)
        if (wasUsingTwoBytes) {
            // Convertir los dos bytes por canal en un único valor de 16 bits (little-endian)
            originalRed = image.redChannel[2 * i] | (image.redChannel[2 * i + 1] << 8);
            originalGreen = image.greenChannel[2 * i] | (image.greenChannel[2 * i + 1] << 8);
            originalBlue = image.blueChannel[2 * i] | (image.blueChannel[2 * i + 1] << 8);
        } else {
            // Si la imagen original usaba 1 byte por canal
            originalRed = image.redChannel[i];
            originalGreen = image.greenChannel[i];
            originalBlue = image.blueChannel[i];
        }

        // Escalamos los valores de los canales RGB con el nuevo valor máximo
        int newRed = static_cast<int>(round(originalRed * scalingFactor));
        int newGreen = static_cast<int>(round(originalGreen * scalingFactor));
        int newBlue = static_cast<int>(round(originalBlue * scalingFactor));

        // Si estamos cambiando de 2 bytes por canal a 1 byte por canal (nuevo maxVal < 256)
        if (willUseOneByte) {
            // Reducir a 1 byte por canal, escribimos solo el byte menos significativo (LSB)
            newRedChannel.push_back(static_cast<unsigned char>(newRed));
            newGreenChannel.push_back(static_cast<unsigned char>(newGreen));
            newBlueChannel.push_back(static_cast<unsigned char>(newBlue));
        } else {
            // Si el nuevo valor máximo es mayor o igual a 256, usar 2 bytes por canal
            newRedChannel.push_back(newRed & 0xFF);        // Primer byte (LSB)
            newRedChannel.push_back((newRed >> 8) & 0xFF); // Segundo byte (MSB)
            newGreenChannel.push_back(newGreen & 0xFF);
            newGreenChannel.push_back((newGreen >> 8) & 0xFF);
            newBlueChannel.push_back(newBlue & 0xFF);
            newBlueChannel.push_back((newBlue >> 8) & 0xFF);
        }
    }

    // Abrimos el archivo de salida en modo binario
    std::ofstream outFile(outputFile, std::ios::binary);
   
    // Escribir los metadatos de la imagen en el archivo PPM
    outFile << "P6\n" << image.width << " " << image.height << "\n" << newMaxLevel << "\n";

    // Escribir los nuevos píxeles escalados en el archivo
    if (willUseOneByte) {
        // Escribir los datos si el nuevo formato es de 1 byte por canal
        for (int i = 0; i < newRedChannel.size(); ++i) {
            outFile.put(newRedChannel[i]);
        }
        for (int i = 0; i < newGreenChannel.size(); ++i) {
            outFile.put(newGreenChannel[i]);
        }
        for (int i = 0; i < newBlueChannel.size(); ++i) {
            outFile.put(newBlueChannel[i]);
        }
    } else {
        // Escribir los datos si el nuevo formato es de 2 bytes por canal
        for (int i = 0; i < newRedChannel.size(); ++i) {
            outFile.put(newRedChannel[i]);
        }
        for (int i = 0; i < newGreenChannel.size(); ++i) {
            outFile.put(newGreenChannel[i]);
        }
        for (int i = 0; i < newBlueChannel.size(); ++i) {
            outFile.put(newBlueChannel[i]);
        }
    }

    // Cerramos el archivo de salida
    outFile.close();
   
    std::cout << "Escalado de intensidad completado. Imagen guardada en: " << outputFile << std::endl;
}



void resize_image_soa(ImageSoa& img, int width, int height) {
    std::vector<unsigned char> new_r(width * height);
    std::vector<unsigned char> new_g(width * height);
    std::vector<unsigned char> new_b(width * height);
    double x_ratio = static_cast<double>(img.width) / width;
    double y_ratio = static_cast<double>(img.height) / height;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int px = static_cast<int>(x * x_ratio);
            int py = static_cast<int>(y * y_ratio);
            new_r[y * width + x] = img.r[py * img.width + px];
            new_g[y * width + x] = img.g[py * img.width + px];
            new_b[y * width + x] = img.b[py * img.width + px];
        }
    }
    img.width = width;
    img.height = height;
    img.r = std::move(new_r);
    img.g = std::move(new_g);
    img.b = std::move(new_b);
}

void cut_least_freq_soa(ImageSoa& img, int n) {
    std::map<std::tuple<unsigned char, unsigned char, unsigned char>, int> freq_map;
    for (int i = 0; i < img.width * img.height; ++i) {
        freq_map[std::make_tuple(img.r[i], img.g[i], img.b[i])]++;
    }

    std::vector<std::pair<std::tuple<unsigned char, unsigned char, unsigned char>, int>> freq_vec(freq_map.begin(), freq_map.end());
    std::sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    std::vector<std::tuple<unsigned char, unsigned char, unsigned char>> least_freq_pixels;
    for (int i = 0; i < n && i < freq_vec.size(); ++i) {
        least_freq_pixels.push_back(freq_vec[i].first);
    }

    for (int i = 0; i < img.width * img.height; ++i) {
        if (std::find(least_freq_pixels.begin(), least_freq_pixels.end(), std::make_tuple(img.r[i], img.g[i], img.b[i])) != least_freq_pixels.end()) {
            img.r[i] = 0;
            img.g[i] = 0;
            img.b[i] = 0;
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
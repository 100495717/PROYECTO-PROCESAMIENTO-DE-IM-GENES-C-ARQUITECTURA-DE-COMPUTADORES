#include "ppm-soa.hpp"
#include <fstream>
#include <stdexcept>
#include "imagesoa.hpp"


//readPPM y writePPM en plan cutre, no binario
void readPPM(const std::string& filename, ImageSoa& img) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo de entrada.");
    }

    std::string format;
    file >> format;
    if (format != "P6") {
        throw std::runtime_error("Formato de archivo no compatible. Se requiere P6.");
    }

    file >> img.width >> img.height >> img.max_color_value;
    file.ignore();  // Ignora el carácter de nueva línea después del valor máximo de color

    std::cout << "Leyendo imagen: " << filename << "\n"
              << "Dimensiones: " << img.width << "x" << img.height << "\n"
              << "Valor máximo de color: " << img.max_color_value << std::endl;

    // Reserva espacio en los vectores según el número total de píxeles
    img.redChannel.resize(img.width * img.height);
    img.greenChannel.resize(img.width * img.height);
    img.blueChannel.resize(img.width * img.height);

    // Lectura de píxeles en función de si el valor máximo es mayor a 255 (16 bits) o no (8 bits)
    if (img.max_color_value > 255) {
        // Imagen de 16 bits por canal
        for (size_t i = 0; i < img.width * img.height; ++i) {
            unsigned char high_byte, low_byte;

            // Lee cada canal en dos bytes (16 bits)
            file.read(reinterpret_cast<char*>(&high_byte), 1);
            file.read(reinterpret_cast<char*>(&low_byte), 1);
            img.redChannel[i] = (high_byte << 8) | low_byte;

            file.read(reinterpret_cast<char*>(&high_byte), 1);
            file.read(reinterpret_cast<char*>(&low_byte), 1);
            img.greenChannel[i] = (high_byte << 8) | low_byte;

            file.read(reinterpret_cast<char*>(&high_byte), 1);
            file.read(reinterpret_cast<char*>(&low_byte), 1);
            img.blueChannel[i] = (high_byte << 8) | low_byte;

            if (i < 10) {
                std::cout << "[Debug] Pixel " << i
                          << " - R: " << img.redChannel[i]
                          << ", G: " << img.greenChannel[i]
                          << ", B: " << img.blueChannel[i] << std::endl;
            }
        }
    } else {
        // Imagen de 8 bits por canal
        for (size_t i = 0; i < img.width * img.height; ++i) {
            unsigned char red, green, blue;

            // Lee cada canal en un solo byte (8 bits) y convierte a `unsigned short`
            file.read(reinterpret_cast<char*>(&red), 1);
            file.read(reinterpret_cast<char*>(&green), 1);
            file.read(reinterpret_cast<char*>(&blue), 1);

            img.redChannel[i] = static_cast<unsigned short>(red);
            img.greenChannel[i] = static_cast<unsigned short>(green);
            img.blueChannel[i] = static_cast<unsigned short>(blue);

            if (i < 10) {
                std::cout << "[Debug] Pixel " << i
                          << " - R: " << img.redChannel[i]
                          << ", G: " << img.greenChannel[i]
                          << ", B: " << img.blueChannel[i] << std::endl;
            }
        }
    }

    file.close();
    std::cout << "Lectura completada con éxito." << std::endl;
}

void writePPM(const std::string& filename, const ImageSoa& img) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo de salida.");
    }

    // Encabezado PPM
    file << "P6\n" << img.width << " " << img.height << "\n" << img.max_color_value << "\n";
    std::cout << "Escribiendo imagen escalada en: " << filename << "\n"
              << "Dimensiones: " << img.width << "x" << img.height << "\n"
              << "Valor máximo de color (escalado): " << img.max_color_value << std::endl;

    for (size_t i = 0; i < img.redChannel.size(); ++i) {
        if (img.max_color_value > 255) {
            // Imagen de 16 bits por canal
            file.put(static_cast<char>(img.redChannel[i] >> 8));
            file.put(static_cast<char>(img.redChannel[i] & 0xFF));
            file.put(static_cast<char>(img.greenChannel[i] >> 8));
            file.put(static_cast<char>(img.greenChannel[i] & 0xFF));
            file.put(static_cast<char>(img.blueChannel[i] >> 8));
            file.put(static_cast<char>(img.blueChannel[i] & 0xFF));
        } else {
            // Imagen de 8 bits por canal
            file.put(static_cast<char>(img.redChannel[i]));
            file.put(static_cast<char>(img.greenChannel[i]));
            file.put(static_cast<char>(img.blueChannel[i]));
        }

        if (i < 10) {
            std::cout << "[Debug] Pixel escrito " << i 
                      << " - R: " << img.redChannel[i]
                      << ", G: " << img.greenChannel[i]
                      << ", B: " << img.blueChannel[i] << std::endl;
        }
    }

    file.close();
    std::cout << "Escalado completado. Imagen guardada en " << filename << std::endl;
}

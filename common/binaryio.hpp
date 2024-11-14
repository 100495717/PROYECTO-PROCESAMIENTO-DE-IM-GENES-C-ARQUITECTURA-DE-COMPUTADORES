//Tanto en este documento como en el binaryio.cpp se van a implementar funciones para leer y escribir archivos en formato binario.
//Al trabajar con imágenes en formato PPM, las operaciones de lectura y escritura binaria van a ser fundamentales, para poder procesar eficientemente los archivos.

#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <string>
#include <vector>
#include <cstdint>

class BinaryIO {
public:
    // Función que abre un archivo binario, lo lee y devuelve su contenido como vector de bytes.
    static std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    // Escribe un vector de bytes en un archivo binario.
    static void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
};

#endif // BINARYIO_HPP
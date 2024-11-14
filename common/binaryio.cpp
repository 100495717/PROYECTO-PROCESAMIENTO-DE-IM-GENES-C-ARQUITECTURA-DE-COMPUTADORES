//Implementación de las funciones de BinaryIO
#include "binaryio.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>

std::vector<uint8_t> BinaryIO::readBinaryFile(const std::string& filePath) {
    // Abrir el archivo en modo binario
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for reading: " + filePath);
    }

    // Obtener el tamaño del archivo, para ajustar el tamaño del vector de bytes que se utiliza para leer el contenido
    file.seekg(0, std::ios::end);
    const std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Lee el contenido del archivo y lo almacena en un vector de bytes
    std::vector<uint8_t> buffer(static_cast<std::vector<uint8_t>::size_type>(fileSize));
     // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        throw std::runtime_error("Error: Unable to read data from file: " + filePath);
    }

    //Cerrar el archivo
    file.close();
    return buffer;//Devuelve el vector de bytes
}

//Función que recibe una ruta de archivo y un vector, este vector contiene la información en bytes que se desea escribir en el archivo.
void BinaryIO::writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    // Abrir el archivo en modo binario
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for writing: " + filePath);
    }

    // Escribir el contenido del vector en el archivo
    const auto dataSize = static_cast<std::streamsize>(data.size());
     // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (!file.write(reinterpret_cast<const char*>(data.data()), dataSize)) {
        throw std::runtime_error("Error: Unable to write data to file: " + filePath);
    }

    file.close();
}
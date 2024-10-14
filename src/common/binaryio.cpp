#include "binaryio.hpp"
// Necesario para la lectura y escritura de archivos
#include <fstream>
#include <stdexcept>

// Función que toma como argumento el nombre de un archivo y devuelve los datos en un vector de bytes
std::vector<uint8_t> read_binary_file(const std::string& filename) {
    // Abrimos el archivo en modo binario (modo lectura)
    std::ifstream file(filename, std::ios::binary);
    
    // Verificamos si el archivo se ha abierto correctamente
    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo para lectura: " + filename);
    }

    // Mover el puntero de lectura al final de archivo
    file.seekg(0, std::ios::end);
    // Obtenemos la posición actual del puntero de lectura, que en este caso será el tamaño del archivo (porque estamos en el final)
    std::streamsize size = file.tellg();
    // Movemos el puntero de lectura al inicio del archivo para comenzar a leer los datos
    file.seekg(0, std::ios::beg);

    // Creamos un buffer del tamaño del archivo
    std::vector<uint8_t> buffer(size);
    // Convertimos el buffer en un puntero a char y leemos los datos del archivo
    // NOLINTNEXTLINE(cppcoreguidelines−pro−type−reinterpret−cast)
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error: Fallo al leer el archivo: " + filename);
    }

    // Devolvemos el buffer con los datos binarios
    return buffer;
}

// Función que dado un vector de bytes y un nombre de archivo, escribe los datos binarios en el archivo
void write_binary_file(const std::string& filename, const std::vector<uint8_t>& data) {
    // Abrimos/creamos el archivo en modo binario (modo escritura)
    std::ofstream file(filename, std::ios::binary);

    // Verificamos si el archivo se abrió correctamente
    if (!file) {
        throw std::runtime_error("Error: No se pudo abrir el archivo para escritura: " + filename);
    }

    // Convertimos el vector de bytes (data) en un puntero a char y escribimos su contenido en el archivo
    // NOLINTNEXTLINE(cppcoreguidelines−pro−type−reinterpret−cast)
    if (!file.write(reinterpret_cast<const char*>(data.data()), data.size())) {
        throw std::runtime_error("Error: Fallo al escribir en el archivo: " + filename);
    }
}


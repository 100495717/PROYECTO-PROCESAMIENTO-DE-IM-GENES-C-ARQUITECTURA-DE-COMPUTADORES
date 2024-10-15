#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

/*  Implementamos esta clase directamente en la cabecera debido a que sus funciones son simples y 
    pequeñas lo que hace que sean buenas candidadtas para inlining (el compilador reemplaza la llamada 
    a la función por el código de la función, lo que puede mejorar el rendimiento)
*/

// Clase para leer datos binarios de un vector de bytes
class BinaryReader {
public:
    // Constructor que alamcena el vector de bytes en una variable llamada data e inicializa la 
    // posición actual en el buffer a 0
    BinaryReader(const std::vector<uint8_t>& buffer) : data(buffer), position(0) {}

    // Función que lee un byte del buffer y lo devuelve
    unsigned char read_byte() {
        // Comprobamos que estamos dentro de los límites del buffer
        if (position >= data.size()) {
            throw std::runtime_error("Lectura fuera de los límites del buffer");
        }
        // Avanzamos una posición en el buffer y devolvemos el byte en la posición anterior
        return data[position++];
    }

    // Funcion que lee un entero en formato little-endian (2 bytes)
    int read_word_le() {
        // Comprobamos que estamos dentro de los límites del buffer
        if (position + 2 > data.size()) {
            throw std::runtime_error("Lectura fuera de los límites del buffer");
        }
        // Combinamos los dos bytes en un entero de 16 bits
        int word = data[position] | (data[position + 1] << 8);
        // Avanzamos dos posiciones en el buffer
        position += 2;
        // Devolvemos el valor de los dos bytes combinados como un entero
    }

    // Leer una cadena ASCII de tamaño 'length'
    std::string read_ascii_string(size_t length) {
        // Comprobamos que estamos dentro de los límites del buffer
        if (position + length > data.size()) {
            throw std::runtime_error("Lectura fuera de los límites del buffer");
        }
        // Creamos una cadena a partir de los bytes en el rango dado por parámetro
        std::string str(data.begin() + position, data.begin() + position + length);
        // Avanzamos la posición actual en el buffer la cantidad de bytes leída
        position += length;
        // Devolvemos la cadena ASCII formada por los bytes leídos
        return str;
    }

    // Función que lee un entero en formato ASCII (terminado por un espacio o salto de línea)
    int read_ascii_int() {
        std::string num_str;
        // Leemos caracter por caracter hasta encontrar un espacio o salto de línea
        while (position < data.size() && (data[position] != ' ' && data[position] != '\n')) {
            num_str += static_cast<char>(data[position++]);
        }
        // Saltamos el espacio o salto de línea
        position++;  
        // Devolvemos la cadena convertida a entero
        return std::stoi(num_str);
    }

    // Función que salta una cantidad de bytes
    void skip(size_t count) {
        // Comprobamos que estamos dentro de los límites del buffer
        if (position + count > data.size()) {
            throw std::runtime_error("Lectura fuera de los límites del buffer");
        }
        // Avanzamos la posición actual en el buffer la cantidad de bytes dada por parámetro
        position += count;
    }

private:
    const std::vector<uint8_t>& data;  // Buffer de bytes
    size_t position;  // Posición actual en el buffer
};



void write_binary_file(const std::string& filename, const std::vector<uint8_t>& data);

// uint8_t es un entero sin signo de 8 bits (1 byte)
std::vector<uint8_t> read_binary_file(const std::string& filename);

#endif 
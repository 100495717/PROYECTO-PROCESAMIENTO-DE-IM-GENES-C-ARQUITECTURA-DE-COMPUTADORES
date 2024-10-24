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

    // Función que salta bytes de espacios en blanco y comentarios 
    void skip() {
        while (position < data.size()) {
            char c = data[position];  // Leer el byte actual
            // Si es un espacio en blanco, tabulación o salto de línea
            if (isspace(c)) {
                ++position;  // Avanzamos la posición para saltar el espacio
            }
            // Si es un comentario
            else if (c == '#') {
                // Avanzamos la posición hasta el final de la línea (buscando '\n')
                while (position < data.size() && data[position] != '\n') {
                    ++position;
                }
                if (position < data.size() && data[position] == '\n') {
                    ++position;  // Saltar también el salto de línea final ('\n')
                }
            }
            // Si no es ni espacio ni comentario, salimos del bucle
            else {
                break;
            }
        }
    }

private:
    const std::vector<uint8_t>& data;  // Buffer de bytes
    size_t position;  // Posición actual en el buffer
};

// Clase para escribir datos binarios en un vector de bytes
class BinaryWriter {
public:

/*
OPTIMIZACION posible: Prealocar memoria en el constructor de BinaryWriter:
BinaryWriter(size_t reserve_size = 0) : position(0) {
    if (reserve_size > 0) {
        data.reserve(reserve_size);
    }
}
En el main:
size_t expected_size = 3 * header.width * header.height +  tamaño estimado del header ;
BinaryWriter writer(expected_size);
*/
    // Constructor que inicializa el buffer vacío

    BinaryWriter(std::vector<uint8_t>& buffer) : data(buffer) {}

    // Escribir un byte
    void write_byte(uint8_t value) {
        data.push_back(value);
    }

    // Escribir un entero en formato little-endian (2 bytes)
    void write_word_le(uint16_t value) {
        data.push_back(static_cast<uint8_t>(value & 0xFF));
        data.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    }

    // Escribir una cadena ASCII
    void write_ascii_string(const std::string& str) {
        data.insert(data.end(), str.begin(), str.end());
    }

    // Escribir un entero en formato ASCII
    void write_ascii_int(int value) {
        write_ascii_string(std::to_string(value));
    }

    // Escribir un carácter de espacio o nueva línea
    void write_whitespace() {
        write_byte(' ');
    }

    void write_newline() {
        write_byte('\n');
    }

private:
    std::vector<uint8_t>& data;  // Buffer donde escribimos los datos
};

void write_binary_file(const std::string& filename, const std::vector<uint8_t>& data);

// uint8_t es un entero sin signo de 8 bits (1 byte)
std::vector<uint8_t> read_binary_file(const std::string& filename);

#endif
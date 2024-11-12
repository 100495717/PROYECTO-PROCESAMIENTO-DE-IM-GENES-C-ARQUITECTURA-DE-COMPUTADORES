#include "binaryio.hpp"
#include <fstream>


BinaryReader::BinaryReader(const std::vector<uint8_t>& buffer)
    : buffer_(buffer), position_(0) {}

uint8_t BinaryReader::read_uint8() {
   
    return buffer_[position_++];
}

uint16_t BinaryReader::read_uint16() {
   
    uint16_t high_byte = static_cast<uint16_t>(buffer_[position_]);
    uint16_t low_byte = static_cast<uint16_t>(buffer_[position_ + 1]);
    uint16_t value = high_byte | low_byte;
    position_ += 2;
    return value;
}

std::string BinaryReader::read_string() {
    std::string result;
    while (position_ < buffer_.size() && buffer_[position_] != ' ' && buffer_[position_] != '\n') {
        result += static_cast<char>(buffer_[position_++]);
    }
    return result;
}

// Función que salta bytes de espacios en blanco y comentarios
void BinaryReader::skip() {
    while (position_ < buffer_.size()) {
        unsigned char uc = buffer_[position_];
        char c = static_cast<char>(uc);

        // Si es un espacio en blanco o tabulación o salto de línea
        if (isspace(c)) {
            ++position_;
        }
        // Si es un comentario
        else if (c == '#') {
            // Avanzar hasta el final de la línea
            while (position_ < buffer_.size() && buffer_[position_] != '\n') {
                ++position_;
            }
            if (position_ < buffer_.size() && buffer_[position_] == '\n') {
                ++position_; // Saltar también el salto de línea
            }
        }
        // Si no es espacio ni comentario, salimos del bucle
        else {
            break;
        }
    }
}

int BinaryReader::read_ascii_int() {
    std::string result;
    while (position_ < buffer_.size() && isdigit(buffer_[position_])) {
        result += static_cast<char>(buffer_[position_++]);
    }
    return std::stoi(result);
}

bool BinaryReader::eof() const {
    return position_ >= buffer_.size();
}

BinaryWriter::BinaryWriter(std::ofstream& output_stream)
    : ofs_(output_stream) {
    if (!ofs_) {
        throw std::ios_base::failure("Error al abrir el archivo de salida");
    }
}

void BinaryWriter::write_uint8(uint8_t value) const {
    if (!ofs_) {
        throw std::ios_base::failure("Error al escribir en el archivo de salida");
    }
    ofs_.write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
}

void BinaryWriter::write_uint16(uint16_t value) const {
    if (!ofs_) {
        throw std::ios_base::failure("Error al escribir en el archivo de salida");
    }
    ofs_.write(reinterpret_cast<const char*>(&value), sizeof(uint16_t));
}

void BinaryWriter::write_string(const std::string& value) const {
    if (!ofs_) {
        throw std::ios_base::failure("Error al escribir en el archivo de salida");
    }
    ofs_.write(value.c_str(), static_cast<std::streamsize>(value.size()));
}

void BinaryWriter::write_ascii_int(int value) const {
    if (!ofs_) {
        throw std::ios_base::failure("Error al escribir en el archivo de salida");
    }
    ofs_ << value;
}
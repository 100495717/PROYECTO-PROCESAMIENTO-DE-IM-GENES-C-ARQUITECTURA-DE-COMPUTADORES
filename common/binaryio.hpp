#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <vector>
#include <string>
#include <cstdint>

class BinaryReader {
public:
    explicit BinaryReader(const std::vector<uint8_t>& buffer);

    // Lee un byte sin signo
    uint8_t read_uint8();
    // Lee dos bytes sin signo en formato big-endian
    uint16_t read_uint16();
    // Lee un string hasta el próximo espacio o salto de línea
    std::string read_string();
    // Leer un int de ascii
    int read_ascii_int();
    // Salta espacios en blanco y saltos de línea
    void skip();

    [[nodiscard]] bool eof() const;
private:
    const std::vector<uint8_t>& buffer_;
    size_t position_;
};

class BinaryWriter {
public:
    explicit BinaryWriter(std::ofstream& output_stream);

    void write_uint8(uint8_t value) const;
    void write_uint16(uint16_t value) const;
    void write_string(const std::string& value) const;
    void write_ascii_int(int value) const;

private:
    std::ofstream& ofs_;
};

#endif // BINARYIO_HPP
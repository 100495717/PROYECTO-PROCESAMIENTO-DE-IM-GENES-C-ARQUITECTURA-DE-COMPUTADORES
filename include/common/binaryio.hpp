#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <string>
#include <vector>
#include <cstdint>

void writeBinary(const std::string& filename, const std::vector<uint8_t>& data);

// uint8_t es un entero sin signo de 8 bits (1 byte)
std::vector<uint8_t> readBinary(const std::string& filename);

#endif 
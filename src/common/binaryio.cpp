#include "binaryio.hpp"
#include <fstream>
#include <vector>

void writeBinary(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        throw std::runtime_error("Failed to open file for writing");
    }
    outfile.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::vector<uint8_t> readBinary(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        throw std::runtime_error("Failed to open file for reading");
    }
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(infile), {});
}

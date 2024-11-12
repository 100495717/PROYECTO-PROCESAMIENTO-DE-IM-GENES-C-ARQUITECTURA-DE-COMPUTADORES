#include "../common/binaryio.hpp"
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include <string>


TEST(binaryio, write_uint8_binaryio) {

  
    std::ofstream ofs("write_uint8_test.bin", std::ios::binary);
    BinaryWriter writer(ofs);
    std::vector<uint8_t> const data = {0x01, 0x02, 0x03, 0x04};
    for (auto const& value : data) {
        writer.write_uint8(value);
    }
    ofs.close();

    std::ifstream ifs("write_uint8_test.bin", std::ios::binary);
    std::vector<uint8_t> read_data(data.size());
    ifs.read(reinterpret_cast<char*>(read_data.data()), static_cast<std::streamsize>(read_data.size()));
    ifs.close();
    ASSERT_EQ(data, read_data);
    

   
}

TEST(binaryio, write_uint16_binaryio) {

    std::ofstream ofs("write_uint16_test.bin", std::ios::binary);
    BinaryWriter writer(ofs);
    std::vector<uint16_t> const data = {0x0102, 0x0304, 0x0506, 0x0708};
    for (auto const& value : data) {
        writer.write_uint16(value);
    }
    ofs.close();

    std::ifstream ifs("write_uint16_test.bin", std::ios::binary);
    std::vector<uint16_t> read_data(data.size());
    ifs.read(reinterpret_cast<char*>(read_data.data()), static_cast<std::streamsize>(read_data.size() * sizeof(uint16_t)));
    ifs.close();
    ASSERT_EQ(data, read_data);
    
}

TEST(binaryio, write_string_binaryio) {

    std::ofstream ofs("write_string_test.bin", std::ios::binary);
    BinaryWriter writer(ofs);
    std::string const data = "Hello, world!";
    writer.write_string(data);
    ofs.close();

    std::ifstream ifs("write_string_test.bin", std::ios::binary);
    std::string read_data;
    std::getline(ifs, read_data);
    ifs.close();
    ASSERT_EQ(data, read_data);
    
}

TEST(binaryio, write_ascii_int_binaryio) {

    std::ofstream ofs("write_ascii_int_test.bin", std::ios::binary);
    BinaryWriter writer(ofs);
    int const data = 12345;
    writer.write_ascii_int(data);
    ofs.close();

    std::ifstream ifs("write_ascii_int_test.bin", std::ios::binary);
    int read_data;
    ifs >> read_data;
    ifs.close();
    ASSERT_EQ(data, read_data);
    
}   

TEST(binaryio, read_uint8_binaryio) {

    std::ofstream ofs("read_uint8_test.bin", std::ios::binary);
    std::vector<uint8_t> const data = {0x01, 0x02, 0x03, 0x04};
    for (auto const& value : data) {
        ofs.put(static_cast<char>(value));
    }
    ofs.close();

    //Meto el archivo en un buffer
    std::ifstream ifs("read_uint8_test.bin", std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    BinaryReader reader(buffer);
    std::vector<uint8_t> read_data(data.size());
    for (size_t i = 0; i < read_data.size(); ++i) {
        read_data[i] = reader.read_uint8();
    }
    ASSERT_EQ(data, read_data);
    
}

TEST(binaryio, read_uint16_binaryio) {
    // Escribir datos de prueba en un archivo binario
    std::ofstream ofs("read_uint16_test.bin", std::ios::binary);
    std::vector<uint16_t> const data = {0x0102, 0x0304, 0x0506, 0x0708};
    for (auto const& value : data) {
        uint16_t value_le = htole16(value); // Convertir a little-endian si es necesario
        ofs.write(reinterpret_cast<char const*>(&value_le), sizeof(uint16_t));
    }
    ofs.close();

    // Leer el archivo en un buffer
    std::ifstream ifs("read_uint16_test.bin", std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Usar el buffer para inicializar BinaryReader
    BinaryReader reader(buffer);
    std::vector<uint16_t> read_data(data.size());
    for (size_t i = 0; i < read_data.size(); ++i) {
        read_data[i] = reader.read_uint16();
    }

    // Comparar los datos leídos con los datos originales
    ASSERT_EQ(data, read_data);
}

TEST(binaryio, read_string_binaryio) {
    // Escribir datos de prueba en un archivo binario
    std::ofstream ofs("read_string_test.bin", std::ios::binary);
    std::string const data = "Hello,world!";
    ofs.write(data.c_str(), static_cast<std::streamsize>(data.size()));
    ofs.close();

    // Leer el archivo en un buffer
    std::ifstream ifs("read_string_test.bin", std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Usar el buffer para inicializar BinaryReader
    BinaryReader reader(buffer);
    std::string read_data = reader.read_string();
 

    // Comparar los datos leídos con los datos originales
    ASSERT_EQ(data, read_data);
}

TEST(binaryio, read_ascii_int_binaryio) {
    // Escribir datos de prueba en un archivo binario
    std::ofstream ofs("read_ascii_int_test.bin", std::ios::binary);
    int const data = 12345;
    ofs << data;
    ofs.close();

    // Leer el archivo en un buffer
    std::ifstream ifs("read_ascii_int_test.bin", std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Usar el buffer para inicializar BinaryReader
    BinaryReader reader(buffer);
    int read_data = reader.read_ascii_int();

    // Comparar los datos leídos con los datos originales
    ASSERT_EQ(data, read_data);
}

TEST(binaryio, readuint8_eof_binaryio) {
    std::vector<uint8_t> buffer = {0x01, 0x02, 0x03, 0x04};
    BinaryReader reader(buffer);
    for (size_t i = 0; i < buffer.size(); ++i) {
        reader.read_uint8();
    }
    ASSERT_TRUE(reader.eof());
}

TEST(binaryio, readuint16_eof_binaryio) {
    std::vector<uint8_t> buffer = {0x01, 0x02, 0x03, 0x04};
    BinaryReader reader(buffer);
    for (size_t i = 0; i < buffer.size() / 2; ++i) {
        reader.read_uint16();
    }
    ASSERT_TRUE(reader.eof());
}

#include <iostream>
#include "common/progars.hpp"
#include "../imgaos/ppm-aos.hpp"
#include "imgaos/image-operations-aos.hpp"
#include "common/binaryio.hpp"
#include "common/header_ppm.hpp"
#include <stdexcept>
#include <vector>
#include <string>


int main(int argc, char* argv[]){
    try{
        auto args = parse_arguments(argc, argv);
        std:: string input = args[1];
        std:: string output = args[2];
        std:: string operation = args[3];

        // Leer el archivo binario
        std::vector<uint8_t> data = read_binary_file(input);

        // Crear el lector binario a partir del buffer
        BinaryReader reader(data);

        // Leer el encabezado PPM 
        PPMHeader header = read_ppm_header(reader);

        // Leemos la imagen
        ImageAos img = read_ppm(reader, header);

        // Hacer la operación
        // Si la operación es "info" imprimimos la información de la imagen
        if (operation == "info"){
            print_image_info(img);
        } 

        // Si la operación es "maxlevel" aplicamos el nivel máximo
        else if (operation == "maxlevel") {
            int maxlevel = std::stoi(args[4]);
            max_level(img, maxlevel);
        }

        // Si la operación es "resize" redimensionamos la imagen
        else if (operation == "resize") {
            int width = std::stoi(args[4]);
            int height = std::stoi(args[5]);
            resize_image(img, width, height);
        }

        // Si la operación es "cutfreq" cortamos la frecuencia 
        else if (operation == "cutfreq"){
            int n = std::stoi(args[4]);
            cut_least_freq(img, n);
        } 
        
        // Si la operación es "cutfreq" hacemos una conversión de la imagen al formato cppm
        else if (operation == "compress"){
            compress_image(img, output);
        } 

        // Escribimos la imagen modificada en la salida
        write_ppm(output, img);
    }
    
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
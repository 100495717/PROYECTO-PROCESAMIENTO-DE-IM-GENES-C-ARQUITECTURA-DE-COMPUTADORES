#include <iostream>
#include "../common/progargs.hpp"
#include "../imgaos/image-operations-aos.hpp"
#include "../common/binaryio.hpp"
#include "main.hpp"
#include <stdexcept>
#include <vector>
#include <string>


int main(int argc, char* argv[]){
    try{
        auto args = parse_arguments(argc, argv);
        std:: string input = args[1];
        std:: string output = args[2];
        std:: string operation = args[3];
        //Creo ImageAos vacía
        ImageAos img;
        // Leer la imagen
        load_image_from_file(input, img);

        // Hacer la operación
        if (operation == "compress"){
        
            compress_image(img, output);
        } 
        else{
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
                cut_freq(img, n);
            } 
            else if (operation == "write"){
                write_image_to_file(output, img);
            }
            else{
                throw std::runtime_error("Operación no válida");
            }
        

            // Escribimos la imagen modificada en la salida
            write_image_to_file(output, img);
        }
    }
    
    catch (const std::exception& e){
        std::cerr << "" << e.what() << std::endl;
        return 1;
    }
    return 0;
}
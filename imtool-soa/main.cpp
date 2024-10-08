#include <iostream>
#include <"imgsoa/imageaos.hpp">
#include "common/progargs.hpp"

int main(int argc, char* argv[]){
    if (argc<4){
        std::cerr << "Los argumentos deben ser: imtool <entrada.ppm> <salida.ppm> <operacion>" << std::endl;
    }
    std:: string input = argv[1];
    std:: string output = argv[2];
    std:: string operation = argv[3];

    //Leemos la imagen
    ImageAos img = read_ppm(input);

    //Hacer la operación, por implementar

    //Escribimos la imagen modificada en la salida
    write_ppm(output, img);
    return 0;
}
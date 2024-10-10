#include "progars.hpp"
#include <stdexcept>
#include <string>

//parse_arguments convierte los argumentos de argv[] en un vector de cadenas (std::vector<std::string>)
std::vector<std::string> parse_arguments(int argc, char*argv[]){
    // Si hay menos de 3 argumentos lanzamos la excepción
    if (argc < 4){
        throw std::invalid_argument("Los argumentos deben ser: imtool <entrada.ppm> <salida.ppm> <operacion>");
    }
    return std::vector<std::string>(argv, argv+argc);
}



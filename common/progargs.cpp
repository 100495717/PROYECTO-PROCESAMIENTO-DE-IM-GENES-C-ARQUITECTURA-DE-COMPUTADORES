#include "progargs.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib> // Para std::exit

// Función que comprueba los argumentos para cada opción y retorna -1 en caso de error
int validate_arguments(const std::vector<std::string>& args, int argc) {
    std::string operation = args[3];

    if (operation == "info") {
        if (argc != 4) {
            std::cerr << "Error: Invalid extra arguments for info: " + args[4] << std::endl;
            return -1;
        }
    } else if (operation == "maxlevel") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of arguments for maxlevel: " + std::to_string(argc - 4) << std::endl;
            return -1;
        }
        try {
            int maxlevel = std::stoi(args[4]);
            if (maxlevel <= 0 || maxlevel > 65535) {
                std::cerr << "Error: Invalid maxlevel: " + std::to_string(maxlevel) << std::endl;
                return -1;
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid maxlevel: " + args[4] << std::endl;
            return -1;
        }
    } else if (operation == "resize") {
        if (argc != 6) {
            std::cerr << "Error: Invalid number of extra arguments for resize: " + std::to_string(argc - 4) << std::endl;
            return -1;
        }
        try {
            int width = std::stoi(args[4]);
            int height = std::stoi(args[5]);
            if (width <= 0) {
                std::cerr << "Error: Invalid resize width: " + args[4] << std::endl;
                return -1;
            }
            if (height <= 0) {
                std::cerr << "Error: Invalid resize height: " + args[5] << std::endl;
                return -1;
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid resize dimensions: " + args[4] + ", " + args[5] << std::endl;
            return -1;
        }
    } else if (operation == "cutfreq") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of arguments for cutfreq: " + std::to_string(argc - 4) << std::endl;
            return -1;
        }
        try {
            int cutfreq = std::stoi(args[4]);
            if (cutfreq <= 0) {
                std::cerr << "Error: Invalid cutfreq: " + std::to_string(cutfreq) << std::endl;
                return -1;
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid cutfreq: " + args[4] << std::endl;
            return -1;
        }
    } else if (operation == "compress") {
        if (argc != 4) {
            std::cerr << "Error: Invalid extra arguments for compress: " + args[4] << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Invalid option: " + operation << std::endl;
        return -1;
    }
    return 0;
}

// parse_arguments convierte los argumentos de argv[] en un vector de cadenas (std::vector<std::string>)
std::vector<std::string> parse_arguments(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Error: Invalid number of arguments: " << argc - 1 << std::endl;
        std::exit(-1);
    }
    std::vector<std::string> args(argv, argv + argc);
    if (validate_arguments(args, argc) == -1) {
        std::exit(-1);
    }
    return args;
}
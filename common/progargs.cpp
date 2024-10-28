#include "progargs.hpp"
#include <stdexcept>
#include <string>

// parse_arguments convierte los argumentos de argv[] en un vector de cadenas (std::vector<std::string>)
std::vector<std::string> parse_arguments(int argc, char*argv[]){
    // Si hay menos de 4 argumentos lanzamos la excepción
    if (argc < 4){
        throw std::invalid_argument("Error: Invalid number of arguments: " + std::to_string(argc - 1));
    }
    std::vector<std::string> args(argv, argv+argc);
    std::string operation = args[3];
    // Validamos los argumentos para la operación "info"
    if (operation == "info"){
            if (argc != 4){
                throw std::invalid_argument("Error: Invalid extra arguments for info: " + args[4]);
            }
            // CASO DE QUE SI SE CONTEMPLEN N ARGUMENTOS ERRONEOS
            /*
                std::string extra_args = "";
                for (int i = 4; i < argc; ++i) {
                    extra_args += args[i] + " ";
                }
                throw std::invalid_argument("Error: Invalid extra arguments for info: " + extra_args);
            */
    }

    // Validamos los argumentos para la operación "maxlevel"
    else if (operation == "maxlevel"){
        //Comprobamos que el número de argumentos es el correcto, sino devolvemos error
        if(argc!=5){
            throw std::invalid_argument("Error: Invalid number of arguments for maxlevel: " + std::to_string(argc-4));
        }
        //Pasamos maxsize de string a entero
        try{
            int maxlevel = std::stoi(args[4]);
            //Comprobamos que el valor de maxsize esta entre los valores permitidos
            if(maxlevel <= 0 || maxlevel > 65535){
                throw std::invalid_argument("Error: Invalid maxlevel: " + std::to_string(maxlevel));
            }
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Error: Invalid maxlevel: " + args[4]); // Lanzar la excepción correctamente
        }
    }

    // Validdamos los argumentos para la operación "resize"
    else if (operation == "resize"){
        // Comprobamos primero que el número de argumentos sea corrrecto
        if (argc != 6) {
            throw std::invalid_argument("Error: Invalid number of extra arguments for resize: " + std::to_string(argc - 4));
        }
        // Ahora validamos que los datos introducidos sean correctos
        int width = std::stoi(args[4]);
        int height = std::stoi(argv[5]);
        if (width <= 0){
            throw std::invalid_argument("Error: Invalid resize width: " + args[4]);
        }
        if (height <= 0){
            throw std::invalid_argument("Error: Invalid resize height: " + args[5]);
        }
    }

    // Validamos los argumentos para la operación "cutfreq"
    else if (operation == "cutfreq"){
        //Comprobamos que el número de argumentos es el correcto, sino devolvemos error
        if(argc!=5){
            throw std::invalid_argument("Error: Invalid number of arguments for maxlevel: " + std::to_string(argc-4));
        }
        //Pasamos maxsize de string a entero
        try{
            int cutfreq = std::stoi(args[4]);
            //Comprobamos que el valor de maxsize esta entre los valores permitidos
            if(cutfreq <= 0){
                throw std::invalid_argument("Error: Invalid cutfreq: " + std::to_string(cutfreq));
            }
        }
        catch(const std::invalid_argument&){
             throw std::invalid_argument("Error: Invalid cutfreq: " + args[4]);
        }
    }

    // Validamos los argumentos para la operación "compress"
    else if (operation == "compress"){
       if (argc != 4){
                throw std::invalid_argument("Error: Invalid extra arguments for compress: " + args[4]);
            }
            // CASO DE QUE SI SE CONTEMPLEN N ARGUMENTOS ERRONEOS
            /*
                std::string extra_args = "";
                for (int i = 4; i < argc; ++i) {
                    extra_args += args[i] + " ";
                }
                throw std::invalid_argument("Error: Invalid extra arguments for compress: " + extra_args);
            */
    }

    // Si la operación no es ninguna de las anteriores, no es válida
    else {
        throw std::invalid_argument("Error: Invalid option: " + operation);
    }
    return args;
}



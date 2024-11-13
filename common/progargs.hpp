#ifndef PROGARS_HPP
#define PROGARS_HPP

#include <string>
#include <vector>


//Parsear los argumentos

int validate_arguments(const std::vector<std::string>& args, int argc);
std::vector<std::string> parse_arguments(int argc, char* argv[]);


#endif //PROGARS_HPP


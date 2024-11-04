#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>


//Creamos la clase ImageSoa donde vamos a almacenar los datos de la imagen PPM
//Separamos los canales de los colores en tres arrays distintos
struct ImageSoa {
public:
    int width;
    int height;
    int max_color_value;
    std::vector <unsigned short> redChannel;
    std::vector <unsigned short> greenChannel;
    std::vector <unsigned short> blueChannel;
};

#endif // IMAGESOA_HPP

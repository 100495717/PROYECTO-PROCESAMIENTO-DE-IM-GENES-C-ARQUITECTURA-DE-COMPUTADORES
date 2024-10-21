#include "ppm-soa.hpp"
#include <fstream>
#include <stdexcept>
#include "imagesoa.hpp"

ImageSoa read_ppm(const std::string& filename){
    //Comprobamos que podemos abrir el archivo de entrada
    std:: ifstream file(filename, std::ios::binary);
    if(!file.is_open()){
        throw std::runtime_error("No se pudo abrir el archivo para lectura");
    }

    //Comprobamos que el formato es el correcto (P6)
    std::string magic;
    file >> magic;
    if (magic != "P6"){
        throw std::runtime_error("Formato de archivo no válido");
    }

    //Leemos y guardamos los valores ancho alto y maxval
    int width, height, maxval;
    file >> width >> height >> maxval;
    file.ignore(1); //Ignoramos el salto de linea despues del valor max

    //Creamos un objeto de la clase ImageSoa
    ImageSoa image;
    image.width = width;
    image.height = height;
    image.max_color_value = maxval;

    int numPixeles = width * height;

    //Redimensionamos los vectores (Arrays) 
    image.redChannel.resize(numPixeles);
    image.greenChannel.resize(numPixeles);
    image.blueChannel.resize(numPixeles);


    //Leemos y guardamos los pixeles (DEPENDE DEL MAXVAL)
    if (maxval <= 255){
        for(int i = 0; i<numPixeles; i++){
            unsigned char r, g, b;
            file.read(reinterpret_cast<char*>(&r), 1) //Leemos 1 byte para el rojo
            file.read(reinterpret_cast<char*>(&g), 1) //Leemos 1 byte para el verde
            file.read(reinterpret_cast<char*>(br), 1) //Leemos 1 byte para el azul

            image.redChannel[i] = static_cast<int>(r); //Lo convertimos a entero y guardamos
            image.greenChannel[i] = static_cast<int>(g);
            image.blueChannel[i] = static_cast<int>(b);

        }
    }
    else{
    //Si maxval > 255 utilizamos dos bytes para cada pixel
        for(int i = 0; i<numPixeles; i++){
            unsigned char r1, r2, g1, g2, b1, b2;
            file.read(reinterpret_cast<char*>(&r1), 1); //Leemos el primer byte para el rojo
            file.read(reinterpret_cast<char*>(&r2), 1); //Leemos el segundo byte para el rojo
            file.read(reinterpret_cast<char*>(&g1), 1);
            file.read(reinterpret_cast<char*>(&g2), 1); 
            file.read(reinterpret_cast<char*>(&b1), 1); 
            file.read(reinterpret_cast<char*>(&b2), 1);

            //Combinamos los 2 bytes para formar el valo de 16 bits
            image.redChannel[i] = static_cast<int>(r1 | (r2 << 8)); //Lo convertimos a entero y guardamos
            image.greenChannel[i] = static_cast<int>(g1 | (g2 << 8));
            image.blueChannel[i] = static_cast<int>(b1 | (b2 << 8));

        }
    }

    void savePPM(const std::string& outputFile, const ImageSOA& image) {
    std::ofstream file(outputFile, std::ios::binary);  // Abrir el archivo en modo binario
   
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << outputFile << std::endl;
        exit(1);
    }
   
    // Escribir el encabezado PPM
    file << "P6\n" << image.width << " " << image.height << "\n" << image.maxColorValue << "\n";
   
    int pixelCount = image.width * image.height;
   
    if (image.maxColorValue <= 255) {
        // Si el valor máximo es <= 255, cada canal usa 1 byte por píxel
        for (int i = 0; i < pixelCount; ++i) {
            unsigned char r = static_cast<unsigned char>(image.redChannel[i]);
            unsigned char g = static_cast<unsigned char>(image.greenChannel[i]);
            unsigned char b = static_cast<unsigned char>(image.blueChannel[i]);
           
            file.write(reinterpret_cast<char*>(&r), 1);  // Escribir 1 byte para el rojo
            file.write(reinterpret_cast<char*>(&g), 1);  // Escribir 1 byte para el verde
            file.write(reinterpret_cast<char*>(&b), 1);  // Escribir 1 byte para el azul
        }
    } else {
        // Si el valor máximo es > 255, cada canal usa 2 bytes por píxel (little-endian)
        for (int i = 0; i < pixelCount; ++i) {
            unsigned char r1 = image.redChannel[i] & 0xFF;       // Primer byte del rojo
            unsigned char r2 = (image.redChannel[i] >> 8) & 0xFF; // Segundo byte del rojo
            unsigned char g1 = image.greenChannel[i] & 0xFF;     // Primer byte del verde
            unsigned char g2 = (image.greenChannel[i] >> 8) & 0xFF; // Segundo byte del verde
            unsigned char b1 = image.blueChannel[i] & 0xFF;      // Primer byte del azul
            unsigned char b2 = (image.blueChannel[i] >> 8) & 0xFF; // Segundo byte del azul
           
            file.write(reinterpret_cast<char*>(&r1), 1);  // Escribir el primer byte del rojo
            file.write(reinterpret_cast<char*>(&r2), 1);  // Escribir el segundo byte del rojo
            file.write(reinterpret_cast<char*>(&g1), 1);  // Escribir el primer byte del verde
            file.write(reinterpret_cast<char*>(&g2), 1);  // Escribir el segundo byte del verde
            file.write(reinterpret_cast<char*>(&b1), 1);  // Escribir el primer byte del azul
            file.write(reinterpret_cast<char*>(&b2), 1);  // Escribir el segundo byte del azul
        }
    }
}
}

//Creamos una funcion para guardar la imagen en un archivo PPM
void savePPM(const std::string& outputFile, const ImageSOA& image) {
    std::ofstream file(outputFile, std::ios::binary);  // Abrir el archivo en modo binario
   
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << outputFile << std::endl;
        exit(1);
    }
   
    // Escribir el encabezado PPM
    file << "P6\n" << image.width << " " << image.height << "\n" << image.maxColorValue << "\n";
   
    int pixelCount = image.width * image.height;
   
    if (image.maxColorValue <= 255) {
        // Si el valor máximo es <= 255, cada canal usa 1 byte por píxel
        for (int i = 0; i < pixelCount; ++i) {
            unsigned char r = static_cast<unsigned char>(image.redChannel[i]);
            unsigned char g = static_cast<unsigned char>(image.greenChannel[i]);
            unsigned char b = static_cast<unsigned char>(image.blueChannel[i]);
           
            file.write(reinterpret_cast<char*>(&r), 1);  // Escribir 1 byte para el rojo
            file.write(reinterpret_cast<char*>(&g), 1);  // Escribir 1 byte para el verde
            file.write(reinterpret_cast<char*>(&b), 1);  // Escribir 1 byte para el azul
        }
    } else {
        // Si el valor máximo es > 255, cada canal usa 2 bytes por píxel (little-endian)
        for (int i = 0; i < pixelCount; ++i) {
            unsigned char r1 = image.redChannel[i] & 0xFF;       // Primer byte del rojo
            unsigned char r2 = (image.redChannel[i] >> 8) & 0xFF; // Segundo byte del rojo
            unsigned char g1 = image.greenChannel[i] & 0xFF;     // Primer byte del verde
            unsigned char g2 = (image.greenChannel[i] >> 8) & 0xFF; // Segundo byte del verde
            unsigned char b1 = image.blueChannel[i] & 0xFF;      // Primer byte del azul
            unsigned char b2 = (image.blueChannel[i] >> 8) & 0xFF; // Segundo byte del azul
           
            file.write(reinterpret_cast<char*>(&r1), 1);  // Escribir el primer byte del rojo
            file.write(reinterpret_cast<char*>(&r2), 1);  // Escribir el segundo byte del rojo
            file.write(reinterpret_cast<char*>(&g1), 1);  // Escribir el primer byte del verde
            file.write(reinterpret_cast<char*>(&g2), 1);  // Escribir el segundo byte del verde
            file.write(reinterpret_cast<char*>(&b1), 1);  // Escribir el primer byte del azul
            file.write(reinterpret_cast<char*>(&b2), 1);  // Escribir el segundo byte del azul
        }
    }
}



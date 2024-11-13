#include "../imgsoa/image-operations-soa.hpp"

#include <cstdint>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>

class ResizeImageTest : public ::testing::Test {
protected:
    // Configuración de una imagen de prueba pequeña para simplificar la verificación
    void SetUp() override {
        img.width = 2;
        img.height = 2;
        img.r = {255, 0, 255, 0};
        img.g = {0, 255, 0, 255};
        img.b = {0, 0, 255, 255};
    }

    ImageSoa img;
};

TEST_F(ResizeImageTest, ResizeToLarger) {
    resize_image_soa(img, 4, 4);

    EXPECT_EQ(img.width, 4);
    EXPECT_EQ(img.height, 4);
    
    // Verificar píxeles de las esquinas (asumiendo la interpolación es correcta)
    EXPECT_EQ(img.r[0], 255);  // Parte superior izquierda
    EXPECT_EQ(img.g[0], 0);
    EXPECT_EQ(img.b[0], 0);
    
    EXPECT_EQ(img.r[15], 0);   // Parte inferior derecha
    EXPECT_EQ(img.g[15], 255);
    EXPECT_EQ(img.b[15], 255);
}

TEST_F(ResizeImageTest, ResizeToSmaller) {
    resize_image_soa(img, 1, 1);

    EXPECT_EQ(img.width, 1);
    EXPECT_EQ(img.height, 1);
    
    // Verifica que los colores promedio se calculan correctamente en la única posición
    EXPECT_NEAR(img.r[0], 127, 1);  // Valor promedio aproximado
    EXPECT_NEAR(img.g[0], 127, 1);
    EXPECT_NEAR(img.b[0], 127, 1);
}

TEST_F(ResizeImageTest, ResizeToSameSize) {
    resize_image_soa(img, 2, 2);

    EXPECT_EQ(img.width, 2);
    EXPECT_EQ(img.height, 2);

    EXPECT_EQ(img.r[0], 255);
    EXPECT_EQ(img.g[1], 255);
    EXPECT_EQ(img.b[2], 255);
    EXPECT_EQ(img.b[3], 255);
}
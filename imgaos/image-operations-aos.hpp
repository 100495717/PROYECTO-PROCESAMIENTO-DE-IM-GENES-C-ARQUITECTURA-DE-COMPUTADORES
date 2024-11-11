#ifndef IMAGE_OPERATIONS_HPP
#define IMAGE_OPERATIONS_HPP

#include "ppm-aos.hpp"

void print_image_info(const ImageAos& img);
void max_level(ImageAos& img, int maxlevel);
void resize_image(ImageAos& img, int width, int height);
void cut_freq(ImageAos& img, int n);
void compress_image(const ImageAos& img, const std::string& filename);

#endif // IMAGE_OPERATIONS_HPP
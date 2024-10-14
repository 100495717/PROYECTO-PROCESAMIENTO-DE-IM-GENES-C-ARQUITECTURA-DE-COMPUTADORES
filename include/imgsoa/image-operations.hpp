#ifndef IMAGE_OPERATIONS_HPP
#define IMAGE_OPERATIONS_HPP

#include "ppm.hpp"

void print_image_info(const ImageSoa& img);
void max_level(ImageSoa& img, int maxlevel);
void resize_image(ImageSoa& img, int width, int height);
void cut_least_freq(ImageSoa& img, int n);
void compress_image(const ImageSoa& img, const std::string& filename);

#endif // IMAGE_OPERATIONS_HPP
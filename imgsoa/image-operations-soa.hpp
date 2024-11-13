#ifndef IMAGE_OPERATIONS_HPP
#define IMAGE_OPERATIONS_HPP

#include "ppm-soa.hpp"

void print_image_info_soa(const ImageSoa& img);
void max_level(ImageSoa& img, int maxlevel);
void resize_image_soa(ImageSoa& img, int width, int height);
void cutfreq(ImageSoa& img, int n);
void compress_image_soa(const ImageSoa& img, const std::string& filename);

#endif // IMAGE_OPERATIONS_HPP
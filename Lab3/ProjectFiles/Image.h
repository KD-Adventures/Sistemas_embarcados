#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct image {
	uint32_t height;
	uint32_t width;
	const int *array;
} Image;

Image* new_image(const int *array, uint32_t height, uint32_t width);
Image* switch_image(const int *array, uint32_t height, uint32_t width, Image* old_image);

#endif
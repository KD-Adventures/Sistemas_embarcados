#include "Image.h"

Image* new_image(const int *array, uint32_t height, uint32_t width) {
	Image *new_image = (Image*) malloc (sizeof(Image));
	new_image->array = array;
	new_image->height = height;
	new_image->width = width;
	return new_image;
}
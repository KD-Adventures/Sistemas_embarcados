#ifndef DATATYPES
#define DATATYPES

#include <stdint.h>

enum Weather {DAY, NIGHT, SNOW, FOG, SUNSET, SUNRISE};
enum Runway_direction {straight, middle_left, left, middle_right, right};

typedef struct car {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t speed;
	uint32_t color;
} Car;

typedef struct image {
	uint32_t width;
	uint32_t height;
	uint32_t *array;
} Image;

typedef struct image_matrix{
	uint32_t width;
	uint32_t height;
	uint32_t** values;
} Image_matrix;

Image_matrix* new_matrix_image(uint32_t height, uint32_t width);
void delete_matrix_image(Image_matrix* image);

#endif

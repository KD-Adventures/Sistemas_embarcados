#ifndef DATATYPES
#define DATATYPES

#include <stdint.h>

enum Weather {DAY, NIGHT, SNOW, FOG};

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

#endif

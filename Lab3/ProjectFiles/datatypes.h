#ifndef DATATYPES
#define DATATYPES

#include <stdint.h>

enum Weather {DAY, NIGHT, SNOW, FOG, SUNSET, SUNRISE};
enum Runway_direction {straight, middle_left, left, middle_right, right};

typedef struct image {
	uint32_t height;
	uint32_t width;
	uint32_t *array;
} Image;

typedef struct car {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t speed;
	uint32_t color;
	Image* image;
} Car;

typedef struct image_matrix{
	uint32_t height;
	uint32_t width;
	uint32_t** values;
} Image_matrix;

typedef struct mountain {
	uint32_t x_position;
	uint32_t color;
	uint8_t model;
	Image* image;
} Mountain;

typedef struct scenario {
	uint32_t ground;
	uint32_t sky;
	uint32_t horizon;
	uint32_t mountains;
	uint32_t runway;
	enum Weather weather;
} Scenario;

typedef struct console {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t distance;
	// adicionar mais informações
	Image* image;
} Console;

Image_matrix* new_matrix_image(uint32_t height, uint32_t width);
void delete_matrix_image(Image_matrix** image);
void invert_image_axis_x(Image_matrix* image_display);
void invert_image_axis_y(Image_matrix* image_display);

// poderia ser uint8_t

Mountain* new_mountain(uint32_t x_position, uint32_t color, uint8_t model);

void swap(uint32_t* x1, uint32_t* x2);
#endif

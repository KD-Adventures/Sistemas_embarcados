#ifndef DATATYPES
#define DATATYPES

#include <stdlib.h>
#include "Car.h"
#include "Console.h"
#include "Mountain.h"

enum Weather {DAY, NIGHT, SNOW, FOG, SUNSET, SUNRISE};
enum Runway_direction {straight, middle_left, left, middle_right, right};

typedef struct image_matrix{
	uint32_t height;
	uint32_t width;
	uint32_t** values;
} Image_matrix;

typedef struct game_state {
	Car *player_car;
	Car *enemy_car;
	Mountain *mountain;
	Console *console;
	enum Runway_direction runway_direction;
	enum Weather weather;
} GameState;

typedef struct scenario {
	uint32_t ground;
	uint32_t sky;
	uint32_t horizon;
	uint32_t mountains;
	uint32_t runway;
	enum Weather weather;
} Scenario;

Image_matrix* new_matrix_image(uint32_t height, uint32_t width);
void delete_matrix_image(Image_matrix** image);
void invert_image_axis_x(Image_matrix* image_display);
void invert_image_axis_y(Image_matrix* image_display);

void swap(uint32_t* x1, uint32_t* x2);
#endif

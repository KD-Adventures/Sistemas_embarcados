#ifndef GRAPHIC_FUNCTIONS
#define GRAPHIC_FUNCTIONS

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128

#define SKY_HEIGHT 30
#define HORIZON_HEIGHT 8
#define GROUND_HEIGHT 70
#define MENU_HEIGHT 20

#define SKY_Y_POSITION MENU_HEIGHT + GROUND_HEIGHT + HORIZON_HEIGHT
#define HORIZON_Y_POSITION MENU_HEIGHT + GROUND_HEIGHT
#define GROUND_Y_POSITION MENU_HEIGHT
#define MENU_Y_POSITION 0

typedef struct scenario {
	int ground;
	int sky;
	int horizon;
	int mountains;
} Scenario;

#include <stdint.h>
#include <stdbool.h>
#include "datatypes.h"
#include "image_constants.h"
#include "grlib/grlib.h"

void clear_image (Image_matrix* image_memory);
void draw_background (Image_matrix* image_memory, enum Weather weather);
void draw_mountain (uint32_t **image_memory, uint32_t x_position, enum Weather weather);
void draw_car (uint32_t **image_memory, const Car *car, enum Weather weather);

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction);
void draw_line(Image_matrix* image_memory, int x1, int y1, int x2, int y2, int color);

#endif

#ifndef GRAPHIC_FUNCTIONS
#define GRAPHIC_FUNCTIONS


#include <stdint.h>
#include <stdbool.h>
#include "datatypes.h"
#include "image_constants.h"
#include "grlib/grlib.h"


#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128

#define SKY_HEIGHT 30
#define HORIZON_HEIGHT 8
#define GROUND_HEIGHT 65
#define MENU_HEIGHT 25

#define SKY_Y_POSITION MENU_HEIGHT + GROUND_HEIGHT + HORIZON_HEIGHT
#define HORIZON_Y_POSITION MENU_HEIGHT + GROUND_HEIGHT
#define GROUND_Y_POSITION MENU_HEIGHT
#define MENU_Y_POSITION 0

#define RUNWAY_LEFT_START_X_POS 20
#define RUNWAY_RIGHT_START_X_POS 108
#define RUNWAY_END_X_POS 64
#define RUNWAY_END_Y_POS HORIZON_Y_POSITION
#define RUNWAY_START_Y_POS GROUND_Y_POSITION

typedef struct scenario {
	int ground;
	int sky;
	int horizon;
	int mountains;
} Scenario;

void clear_image (Image_matrix* image_memory);
void draw_background (Image_matrix* image_memory, enum Weather weather);
void draw_mountain(Image_matrix* image_memory, const Mountain *mountain, enum Weather weather);
void draw_car (Image_matrix* image_memory, const Car *car, enum Weather weather);
void draw_console (Image_matrix* image_memory, const Console* console);

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction);
void draw_line(Image_matrix* image_memory, int x1, int y1, int x2, int y2, int color);

#endif

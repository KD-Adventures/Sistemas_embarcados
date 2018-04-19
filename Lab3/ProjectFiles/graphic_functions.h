#ifndef GRAPHIC_FUNCTIONS
#define GRAPHIC_FUNCTIONS

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128
#define SKY_HEIGHT 30
#define MENU_HEIGHT 30

#include <stdint.h>
#include <stdbool.h>
#include "datatypes.h"
#include "image_constants.h"
#include "grlib/grlib.h"

void clear_image (uint32_t **image_memory);
void draw_background (uint32_t **image_memory, enum Weather weather);
void draw_mountain (uint32_t **image_memory, uint32_t x_position, enum Weather weather);
void draw_car (uint32_t **image_memory, const Car *car, enum Weather weather);

#endif

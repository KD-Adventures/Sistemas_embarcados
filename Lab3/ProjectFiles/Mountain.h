#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "Image.h"

typedef struct mountain {
	uint32_t x_position;
	uint32_t color;
	uint8_t model;
	Image* image;
} Mountain;

Mountain* new_mountain(uint32_t x_position, uint32_t color, uint8_t model);

extern const int MOUNTAIN_1_ARRAY[];

#endif
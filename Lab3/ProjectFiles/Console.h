#ifndef CONSOLE_H
#define CONSOLE_H

#include "Image.h"

#define N_ENEMIES_GOAL 200
#define LAP_LENGTH 500

typedef struct console {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t distance;
	uint32_t player_position;
	uint32_t race_lap;
	const Image *image;
} Console;

Console* new_console(uint32_t x_position, uint32_t y_position);
extern const int CONSOLE_ARRAY[];

#endif
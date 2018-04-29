#ifndef CONSOLE_H
#define CONSOLE_H

#include "Image.h"

typedef struct console {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t distance;
	const Image *image;
} Console;

Console* new_console(uint32_t x_position, uint32_t y_position);
extern const int CONSOLE_ARRAY[];

#endif
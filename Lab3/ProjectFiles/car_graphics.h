#include <stdint.h>
#include <stdbool.h>
#include "grlib/grlib.h"
#include "cfaf128x128x16.h"

struct car {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t speed;
	uint32_t color;
};

void draw_car (struct car car);
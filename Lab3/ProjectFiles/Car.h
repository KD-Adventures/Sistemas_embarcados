#ifndef CAR_H
#define CAR_H

#include "Image.h"
#include <stdbool.h>

enum Car_direction {LEFT, RIGHT, STRAIGHT};

typedef struct car {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t speed;
	uint32_t color;
	bool accelerating;
	enum Car_direction direction;
	Image* image;
} Car;

extern const int CAR_SMALL_ARRAY[];
extern const int CAR_MEDIUM_ARRAY[];
extern const int CAR_BIG_ARRAY[];

Car *new_car(uint32_t x_position, uint32_t y_position, uint32_t speed, uint32_t color, uint32_t size);

void delete_car(Car* car);

#endif
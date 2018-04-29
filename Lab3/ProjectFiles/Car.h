#ifndef CAR_H
#define CAR_H

#include "Image.h"

typedef struct car {
	uint32_t x_position;
	uint32_t y_position;
	uint32_t speed;
	uint32_t color;
	Image* image;
} Car;

extern const int CAR_SMALL_ARRAY[];
extern const int CAR_MEDIUM_ARRAY[];
extern const int CAR_BIG_ARRAY[];

Car *new_car(uint32_t x_position, uint32_t y_position, uint32_t speed, uint32_t color, uint32_t size);

void delete_car(Car* car);

#endif
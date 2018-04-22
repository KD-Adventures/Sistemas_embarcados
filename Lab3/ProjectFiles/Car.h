#ifndef CAR_H
#define CAR_H

#include "datatypes.h"
#include "image_constants.h"

Car* new_car(uint32_t x_position, uint32_t y_position, uint32_t speed, uint32_t color);

void delete_car(Car* car);

#endif
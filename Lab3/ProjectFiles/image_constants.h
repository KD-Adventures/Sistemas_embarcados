#ifndef IMAGES
#define IMAGES

#include "stdlib.h"
#include "datatypes.h"

extern Image CAR_BIG;
extern Image MOUNTAIN_1;
extern Image CONSOLE;

void load_images();

void load_car_image();

void load_mountain_image(uint8_t model);

#endif

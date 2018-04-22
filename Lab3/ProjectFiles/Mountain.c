#include "Mountain.h"

Mountain* new_mountain(uint32_t x_position, uint32_t color, uint8_t model) {
	Mountain* new_mountain = (Mountain*)malloc(sizeof(Mountain));
	new_mountain->x_position = x_position;
	new_mountain->color = color;
	new_mountain->model = model;
	
	new_mountain->image = &MOUNTAIN_1;
	
	return new_mountain;
}
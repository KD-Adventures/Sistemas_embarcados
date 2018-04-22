#include "Car.h"

Car* new_car(uint32_t x_position, uint32_t y_position, uint32_t speed, uint32_t color) {

	Car* new_car = (Car*)malloc(sizeof(Car));
	new_car->x_position = x_position;
	new_car->y_position = y_position;
	new_car->speed = speed;
	new_car->color = color;
	new_car->image = &CAR_BIG;

	return new_car;
}

void delete_car(Car* car) {
	free(car);
	return;
}
#include "Car.h"

Car *new_car(uint32_t x_position, uint32_t y_position, uint32_t speed, uint32_t color, uint32_t size) {

	Car* new_car = (Car*)malloc(sizeof(Car));
	new_car->x_position = x_position;
	new_car->y_position = y_position;
	new_car->speed = speed;
	new_car->color = color;
	
	switch(size) {
		case 0:
			new_car->image = new_image((const int*) CAR_SMALL_ARRAY, 6, 10);
		break;
		case 1:
			new_car->image = new_image((const int*) CAR_MEDIUM_ARRAY, 9, 18);
		break;
		default:
			new_car->image = new_image((const int*) CAR_BIG_ARRAY, 11, 25);
		break;
	}

	return new_car;
}

void delete_car(Car* car) {
	free(car);
	return;
}

const int CAR_SMALL_ARRAY[] = {
	0x00,0xb6,0xdb,0x92,0xff,0xff,0x92,0xdb,0xb6,0x00
	,0x49,0x92,0xb6,0xb6,0xff,0xff,0xb6,0xdb,0xb6,0x00
	,0x6d,0x92,0xb6,0xdb,0xff,0xff,0xdb,0xb6,0x6d,0x92
	,0x6d,0x92,0xb6,0xff,0xff,0xff,0xff,0xb6,0x6d,0x92
	,0x6d,0x92,0xb6,0xff,0xff,0xff,0xff,0xb6,0x6d,0x92
	,0x6d,0x92,0x6d,0x92,0xff,0xff,0x92,0x6d,0x6d,0x92
};

const int CAR_MEDIUM_ARRAY[] = {
	0x00,0x00,0xb6,0xff,0xb6,0x24,0x6d,0xff,0xff,0xff,0xff,0x6d,0x24,0xb6,0xff,0xb6,0x00,0x00
	,0x00,0x00,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x00,0x00
	,0x92,0x92,0x6d,0x6d,0x6d,0x6d,0x92,0xff,0xff,0xff,0xff,0x92,0x6d,0xdb,0xff,0xb6,0x00,0x00
	,0x49,0x49,0x92,0xb6,0xb6,0xb6,0xdb,0xff,0xff,0xff,0xff,0xdb,0xb6,0x6d,0x49,0x6d,0xb6,0xb6
	,0xff,0xff,0x49,0x00,0x6d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x00,0x00
	,0x49,0x49,0x92,0xb6,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x49,0x6d,0xb6,0xb6
	,0x92,0x92,0x6d,0x6d,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x92,0x92,0x6d,0x6d
	,0xb6,0xb6,0x6d,0x49,0x92,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0xb6,0x92,0x49,0x49
	,0x24,0x24,0x92,0xdb,0x92,0x24,0x6d,0xff,0xff,0xff,0xff,0x6d,0x24,0x24,0x24,0x6d,0xdb,0xdb
};

const int CAR_BIG_ARRAY[] = {
	0x00,0x00,0x00,0xdb,0xff,0xff,0x49,0x00,0x00,0x92,0xff,0xff,0xff,0xff,0xff,0x92,0x00,0x00,0x49,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x00,0x00,0x00
	,0xff,0xff,0xff,0x24,0x00,0x00,0x00,0x00,0x00,0x92,0xff,0xff,0xff,0xff,0xff,0x92,0x00,0x00,0x49,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x00,0x00,0x24,0xff,0xff,0xff
	,0xff,0xff,0xff,0x24,0x00,0x00,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x00,0x00,0x24,0xff,0xff,0xff
	,0xff,0xff,0xff,0x24,0x00,0x00,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb6,0x00,0x00,0x24,0xff,0xff,0xff
	,0xff,0xff,0xff,0x24,0x00,0x00,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x00,0x00,0x00
	,0x00,0x00,0x00,0xdb,0xff,0xff,0x49,0x00,0x00,0x92,0xff,0xff,0xff,0xff,0xff,0x92,0x00,0x00,0x00,0x00,0x00,0x24,0xff,0xff,0xff
};
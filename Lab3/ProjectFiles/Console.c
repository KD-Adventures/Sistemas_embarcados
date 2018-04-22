#include "Console.h"

Console* new_console(uint32_t x_position, uint32_t y_position){

	Console* new_console = (Console*)malloc(sizeof(Console));
	new_console->image = &CONSOLE;
	new_console->x_position = x_position;
	new_console->y_position = y_position;

	return new_console;
}

/*
void delete_car(Car* car) {
	free(car);
	return;
}
*/
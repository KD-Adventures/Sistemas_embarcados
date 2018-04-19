#include "graphic_functions.h"

void clear_image (uint32_t **image_memory) {
	int column, line;

	for (line = 0; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			image_memory[line][column] = ClrBlack;
		}
	}
}

void draw_background (uint32_t **image_memory, enum Weather weather) {
	int column, line;
	
	//Draw sky
	for (line = 0; line < SKY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			if (weather == DAY)
				image_memory[line][column] = ClrBlue;
		}
	}
	
	//Draw ground
	for (line = SKY_HEIGHT; line < DISPLAY_HEIGHT - MENU_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			if (weather == DAY)
				image_memory[line][column] = ClrGreen;
		}
	}
	
	//Draw menu - to do
	for (line = DISPLAY_HEIGHT - MENU_HEIGHT; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			if (weather == DAY)
				image_memory[line][column] = ClrBlack;
		}
	}
}

void draw_mountain (uint32_t **image_memory, uint32_t x_position, enum Weather weather) {
	//To do
}

void draw_car (uint32_t **image_memory, const Car *car, enum Weather weather) {
	//To do
}
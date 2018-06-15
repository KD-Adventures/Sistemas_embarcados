#include "Terminal.h"

void terminal_clear() {
	background_color(0x000000);
	sendString("\033[2J");
}

void goto_xy(uint32_t x, uint32_t y) {
	char command[50];
	char x_value[10];
	char y_value[10];
	
	intToString(x, x_value, 10, 10, 1);
	intToString(y, y_value, 10, 10, 1);
	
	strcpy(command, "\033[");
	strcat(command, y_value);
	strcat(command, ";");
	strcat(command, x_value);
	strcat(command, "f");
	
	sendString(command);
}

void background_color(uint32_t rgb) {
	uint32_t red, green, blue;
	char command[50];
	char r_value[4];
	char g_value[4];
	char b_value[4];
	
	blue = rgb & 0xff;
	green = rgb;
	green = (green >> 8) & 0xff;
	red = rgb;
	red = (red >> 16) & 0xff;
	
	intToString(red, r_value, 10, 10, 1);
	intToString(green , g_value, 10, 10, 1);
	intToString(blue, b_value, 10, 10, 1);
	
	strcpy(command, "\033[48;2;");
	strcat(command, r_value);
	strcat(command, ";");
	strcat(command, g_value);
	strcat(command, ";");
	strcat(command, b_value);
	strcat(command, "m");
	
	sendString(command);
}

void draw_square(uint32_t x_min, uint32_t x_max, uint32_t y_min, uint32_t y_max, uint32_t color) {
	uint32_t i, j, k;
	
	background_color(color);
	goto_xy(x_min, y_min);
	for(i = y_min; i < y_max; i++) {
		goto_xy(x_min, i);
		for (k = 0; k < 1000; k++);
		for(j = x_min; j < x_max; j++) {
			sendString(" ");
		}
	}
	goto_xy(x_max, y_max);
	
}
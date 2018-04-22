#include "graphic_functions.h"

void clear_image (Image_matrix* image_memory) {
	int column, line;

	for (line = 0; line < image_memory->height; line++) {
		for (column = 0; column < image_memory->width; column++) {	
			image_memory->values[line][column] = ClrBlack;
		}
	}
}

void set_weather(enum Weather weather, Scenario* scene) {
	switch (weather) {
		case DAY:
			scene->ground = ClrGreen;
			scene->sky = ClrBlue;
			scene->horizon = ClrBlueViolet;
			scene->mountains = ClrWhite;
		break;
		
		case NIGHT:
			scene->ground = ClrBlack;
			scene->sky = ClrGray;
			scene->horizon = ClrGray;
			scene->mountains = ClrWhite;
		break;
		
		case SNOW:
			scene->ground = ClrWhite;
			scene->sky = ClrBlue;
			scene->horizon = ClrBlue;
			scene->mountains = ClrWhite;
		break;
		
		case SUNSET:
			scene->ground = ClrGreen;
			scene->sky = ClrBlue;
			scene->horizon = ClrViolet;
			scene->mountains = ClrWhite;
		break;
		
		case SUNRISE:
			scene->ground = ClrBlack;
			scene->sky = ClrGray;
			scene->horizon = ClrGray;
			scene->mountains = ClrOrangeRed;
		break;
		
		default:
			scene->ground = ClrGreen;
			scene->sky = ClrBlue;
			scene->horizon = ClrBlueViolet;
			scene->mountains = ClrWhite;
		break;
	}
}

void draw_background (Image_matrix* image_memory, enum Weather weather) {
	int column, line;
	Scenario scene;
	
	set_weather(weather, &scene);
	
	//Draw menu - to do
	for (line = 0; line < MENU_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			image_memory->values[line][column] = ClrBlack;
		}
	}
	
	//Draw ground
	for (line = MENU_HEIGHT; line < MENU_HEIGHT + GROUND_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			image_memory->values[line][column] = scene.ground;
		}
	}
	
	//Draw horizon
	for (line = MENU_HEIGHT + GROUND_HEIGHT; line < DISPLAY_HEIGHT - SKY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			image_memory->values[line][column] = scene.horizon;
		}
	}
	
	//Draw sky
	for (line = DISPLAY_HEIGHT - SKY_HEIGHT; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {	
			image_memory->values[line][column] = scene.sky;
		}
	}
	
	
}


void draw_mountain (uint32_t **image_memory, uint32_t x_position, enum Weather weather) {
	//To do
}

void draw_car (uint32_t **image_memory, const Car *car, enum Weather weather) {
	//To do
	
}

void swap(int* x1, int* x2) {
	int tmp = *x1;
	*x1 = *x2;
	*x2 = tmp;
}

// Peguie da internet, nao testado ainda.
void draw_line(Image_matrix* image_memory, int x1, int y1, int x2, int y2, int color) { 
    bool steep = false; 
    if (abs(x1-x2)<abs(y1-y2)) { 
        swap(&x1, &y1); 
        swap(&x2, &y2); 
        steep = true; 
    } 
    if (x1>x2) { 
        swap(&x1, &x2); 
        swap(&y1, &y2); 
    } 
    float dx = x2-x1; 
    int dy = y2-y1; 
	int div = dy/dx;
    float derror = abs(div); 
    float error = 0; 
    int y = y1; 
    for (int x=x1; x<=x2; x++) { 
        if (steep) { 
            image_memory->values[y][x] = color; 
        } else { 
            image_memory->values[x][y] = color;
        } 
        error += derror; 
        if (error>.5) { 
            y += (y2>y1?1:-1); 
            error -= 1.; 
        } 
    } 
}

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction) {
	
	switch (runway_direction) {
		case straight:
			draw_line(image_memory, GROUND_Y_POSITION, 32, HORIZON_Y_POSITION -1, 62, ClrWhite);
			//GrLineDraw(&sContext, GROUND_Y_POSITION, 32, HORIZON_Y_POSITION -1, 62);
			//GrLineDraw(&sContext, GROUND_Y_POSITION, 94, HORIZON_Y_POSITION -1, 62);
		break;
		
		case left:
			
		break;
		
		case middle_left:
		break;
		
		case right:
		break;
		
		case middle_right:
		break;
	}	
}
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

void draw_image(Image_matrix* image_memory, Image* image, uint8_t pos_x, uint8_t pos_y, uint32_t color, bool original_color, uint8_t color_threshold) {
	int i, j;
	int image_array_index = 0;
	
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++) {
			if(image->array[image_array_index] > color_threshold) {
				if(original_color)
					image_memory->values[i + pos_x][j + pos_y] = image->array[image_array_index];
				else
					image_memory->values[i + pos_x][j + pos_y] = color;
			}
			image_array_index++;
		}
	}
}

void draw_mountain(Image_matrix* image_memory, const Mountain *mountain, enum Weather weather) {
	
	draw_image(image_memory, mountain->image, HORIZON_Y_POSITION, mountain->x_position, mountain->color, false, 150);
}

void draw_car (Image_matrix* image_memory, const Car *car, enum Weather weather) {
	
	draw_image(image_memory, car->image, car->x_position, car->y_position, car->color, false, 150);
}

void draw_console (Image_matrix* image_memory, const Console* console) {
	
	draw_image(image_memory, console->image, console->x_position, console->y_position, 0, true, 0);
}

void swap(int* x1, int* x2) {
	int tmp = *x1;
	*x1 = *x2;
	*x2 = tmp;
}


void draw_line(Image_matrix* image_memory, int32_t i32X1, int32_t i32Y1, int32_t i32X2, int32_t i32Y2, int color)
{
    int32_t i32Error, i32DeltaX, i32DeltaY, i32YStep, bSteep;

    //
    // Determine if the line is steep.  A steep line has more motion in the Y
    // direction than the X direction.
    //
    if(((i32Y2 > i32Y1) ? (i32Y2 - i32Y1) : (i32Y1 - i32Y2)) >
       ((i32X2 > i32X1) ? (i32X2 - i32X1) : (i32X1 - i32X2)))
    {
        bSteep = 1;
    }
    else
    {
        bSteep = 0;
    }

    //
    // If the line is steep, then swap the X and Y coordinates.
    //
    if(bSteep)
    {
        i32Error = i32X1;
        i32X1 = i32Y1;
        i32Y1 = i32Error;
        i32Error = i32X2;
        i32X2 = i32Y2;
        i32Y2 = i32Error;
    }

    //
    // If the starting X coordinate is larger than the ending X coordinate,
    // then swap the start and end coordinates.
    //
    if(i32X1 > i32X2)
    {
        i32Error = i32X1;
        i32X1 = i32X2;
        i32X2 = i32Error;
        i32Error = i32Y1;
        i32Y1 = i32Y2;
        i32Y2 = i32Error;
    }

    //
    // Compute the difference between the start and end coordinates in each
    // axis.
    //
    i32DeltaX = i32X2 - i32X1;
    i32DeltaY = (i32Y2 > i32Y1) ? (i32Y2 - i32Y1) : (i32Y1 - i32Y2);

    //
    // Initialize the error term to negative half the X delta.
    //
    i32Error = -i32DeltaX / 2;

    //
    // Determine the direction to step in the Y axis when required.
    //
    if(i32Y1 < i32Y2)
    {
        i32YStep = 1;
    }
    else
    {
        i32YStep = -1;
    }

    //
    // Loop through all the points along the X axis of the line.
    //
    for(; i32X1 <= i32X2; i32X1++)
    {
        //
        // See if this is a steep line.
        //
        if(bSteep)
        {
            //
            // Plot this point of the line, swapping the X and Y coordinates.
            //
            image_memory->values[i32Y1][i32X1] = color;
        }
        else
        {
            //
            // Plot this point of the line, using the coordinates as is.
            //
            image_memory->values[i32X1][i32Y1] = color;
        }

        //
        // Increment the error term by the Y delta.
        //
        i32Error += i32DeltaY;

        //
        // See if the error term is now greater than zero.
        //
        if(i32Error > 0)
        {
            //
            // Take a step in the Y axis.
            //
            i32Y1 += i32YStep;

            //
            // Decrement the error term by the X delta.
            //
            i32Error -= i32DeltaX;
        }
    }
}



/*
void draw_arc(Image_matrix* image_memory, int center_x, int center_y, int radius, int start_angle, int end_angle) {
    int i = 0;
    for(i = start_angle; i < end_angle; i++)
	{
	 drawpixel(50 + calculatede_cos(i) * radius, y: 100 + calculatede_sin(i) * radius); // center point is (50,100)
	}
 }
 */

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction) {
	

	switch (runway_direction) {
		case straight:
			draw_line(image_memory, RUNWAY_START_Y_POS, RUNWAY_LEFT_START_X_POS, RUNWAY_END_Y_POS -1, RUNWAY_END_X_POS, ClrWhite);
			draw_line(image_memory, RUNWAY_START_Y_POS, RUNWAY_RIGHT_START_X_POS, RUNWAY_END_Y_POS -1, RUNWAY_END_X_POS, ClrWhite);
		break;

		// Nao desenha as curvas no momento, implementar Midpoint circle algorithm para arcos.
		case left:
			draw_line(image_memory, RUNWAY_START_Y_POS, RUNWAY_LEFT_START_X_POS, RUNWAY_END_Y_POS -1, 15, ClrWhite);
			draw_line(image_memory, RUNWAY_START_Y_POS, RUNWAY_RIGHT_START_X_POS, RUNWAY_END_Y_POS -1, 15, ClrWhite);
		break;

		case middle_left:
		break;

		case right:
		break;

		case middle_right:
		break;
	}
}
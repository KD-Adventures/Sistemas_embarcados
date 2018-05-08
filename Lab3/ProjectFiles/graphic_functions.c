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
			scene->runway = ClrWhite;
		break;

		case NIGHT:
			scene->ground = ClrBlack;
			scene->sky = ClrGray;
			scene->horizon = ClrGray;
			scene->mountains = ClrWhite;
			scene->runway = ClrWhite;
		break;

		case SNOW:
			scene->ground = ClrWhite;
			scene->sky = ClrBlue;
			scene->horizon = ClrBlue;
			scene->mountains = ClrWhite;
			scene->runway = ClrBlack;
		break;

		case SUNSET:
			scene->ground = ClrGreen;
			scene->sky = ClrBlue;
			scene->horizon = ClrViolet;
			scene->mountains = ClrWhite;
			scene->runway = ClrWhite;
		break;

		case SUNRISE:
			scene->ground = ClrBlack;
			scene->sky = ClrGray;
			scene->horizon = ClrGray;
			scene->mountains = ClrOrangeRed;
			scene->runway = ClrWhite;
		break;

		default:
			scene->ground = ClrGreen;
			scene->sky = ClrBlue;
			scene->horizon = ClrBlueViolet;
			scene->mountains = ClrWhite;
			scene->runway = ClrWhite;
		break;
	}
}

void draw_background (Image_matrix* image_memory, Scenario* scene) {
	int column, line;

	//Draw menu - to do
	for (line = MENU_Y_POSITION; line < MENU_Y_POSITION + MENU_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			image_memory->values[column][line] = ClrBlack;
		}
	}

	//Draw ground
	for (line = GROUND_Y_POSITION; line < GROUND_Y_POSITION + GROUND_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			image_memory->values[column][line] = scene->ground;
		}
	}

	//Draw horizon
	for (line = HORIZON_Y_POSITION; line < HORIZON_Y_POSITION + HORIZON_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			image_memory->values[column][line] = scene->horizon;
		}
	}

	//Draw sky
	for (line = SKY_Y_POSITION; line < SKY_Y_POSITION + SKY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			image_memory->values[column][line] = scene->sky;
		}
	}
}

void draw_image(Image_matrix* image_memory, const Image* image, uint8_t pos_x, uint8_t pos_y, uint32_t color, bool original_color, uint8_t color_threshold) {
	int i, j;
	int image_array_index = image->height*image->width;
	
	for(j = 0; j < image->height; j++){
		for(i = image->width-1; i >= 0; i--){
			if(image->array[image_array_index] > color_threshold) {
				if( (i+pos_x) < 0 || (i+pos_x) >= image_memory->width || (j+pos_y < 0) || (j+pos_y) >= image_memory->height){
					image_array_index--;
					continue;
				}
				if(original_color)
					image_memory->values[i + pos_x][j + pos_y] = image->array[image_array_index];
				else
					image_memory->values[i + pos_x][j + pos_y] = color;
			}
			image_array_index--;
		}
	}
}

void draw_mountain(Image_matrix* image_memory, const Mountain *mountain, Scenario* scene) {
	
	draw_image(image_memory, mountain->image, mountain->x_position, HORIZON_Y_POSITION, mountain->color, false, 150);
}

float _getPt (float n1 , float n2 , float perc)
{
    float diff = n2 - n1;
    return n1 + ( diff * perc );
}

void draw_bezier_curve (Image_matrix* image_memory, uint32_t color, float x1, float y1, float x2, float y2, float x3, float y3) {
	float i, xa, xb, ya, yb, x, y;
	for (i = 0 ; i < 1 ; i += 0.01)
		{
				xa = _getPt( x1 , x2 , i );
				ya = _getPt( y1 , y2 , i );
				xb = _getPt( x2 , x3 , i );
				yb = _getPt( y2 , y3 , i );

				x = _getPt( xa , xb , i );
				y = _getPt( ya , yb , i );

				image_memory->values[(int)x][(int)y] = color;
		}
}

float bezier_curve_car (float x1, float y1, float x2, float y2, float x3, float y3, uint32_t car_y) {
	float i, xa, xb, ya, yb, x, y;
	for (i = 0 ; i < 1 ; i += 0.01)
		{
				xa = _getPt( x1 , x2 , i );
				ya = _getPt( y1 , y2 , i );
				xb = _getPt( x2 , x3 , i );
				yb = _getPt( y2 , y3 , i );

				x = _getPt( xa , xb , i );
				y = _getPt( ya , yb , i );
				
				if ((int)y >= car_y)
					return x;
		}
	return x3;
}

void draw_car (Image_matrix* image_memory, const Car *car, enum Runway_direction runway_direction) {
	uint32_t car_x_position;
	
	switch(runway_direction) {
		case straight:
			car_x_position = car->runway_x_position;
			if (car->runway_x_position > (DISPLAY_WIDTH / 2)) {
				car_x_position -= ((car->runway_x_position - (DISPLAY_WIDTH / 2)) * (car->runway_y_position - MENU_HEIGHT) / (GROUND_HEIGHT));
			}
			else {
				car_x_position += (((DISPLAY_WIDTH / 2) - car->runway_x_position) * (car->runway_y_position - MENU_HEIGHT) / (GROUND_HEIGHT));
			}
		break;

		case left:
			car_x_position = bezier_curve_car(car->runway_x_position, 20,
			                                  98, 70,
			                                  18, 90,
			                                  car->runway_y_position);
		break;
			
		case right:
			car_x_position = bezier_curve_car(car->runway_x_position, 20,
			                                  30, 70,
			                                  110, 90,
			                                  car->runway_y_position);
		break;
			
		default:
			return;
	}		
	
	draw_image(image_memory, car->image, car_x_position - (car->image->width/2), car->runway_y_position, car->color, false, 150);
}

void draw_console (Image_matrix* image_memory, const Console* console) {
	draw_image(image_memory, console->image, console->x_position, console->y_position, 0, true, 0);
}


void draw_line(Image_matrix* image_memory, int32_t i32X1, int32_t i32Y1, int32_t i32X2, int32_t i32Y2, int color) {
    int32_t i32Error, i32DeltaX, i32DeltaY, i32YStep, bSteep;

    // Determine if the line is steep.  A steep line has more motion in the Y
    // direction than the X direction.
    if(((i32Y2 > i32Y1) ? (i32Y2 - i32Y1) : (i32Y1 - i32Y2)) >
       ((i32X2 > i32X1) ? (i32X2 - i32X1) : (i32X1 - i32X2)))
    {
        bSteep = 1;
    }
    else
    {
        bSteep = 0;
    }

    // If the line is steep, then swap the X and Y coordinates.
    if(bSteep)
    {
        i32Error = i32X1;
        i32X1 = i32Y1;
        i32Y1 = i32Error;
        i32Error = i32X2;
        i32X2 = i32Y2;
        i32Y2 = i32Error;
    }

    // If the starting X coordinate is larger than the ending X coordinate,
    // then swap the start and end coordinates.
    if(i32X1 > i32X2)
    {
        i32Error = i32X1;
        i32X1 = i32X2;
        i32X2 = i32Error;
        i32Error = i32Y1;
        i32Y1 = i32Y2;
        i32Y2 = i32Error;
    }

    // Compute the difference between the start and end coordinates in each
    // axis.
    i32DeltaX = i32X2 - i32X1;
    i32DeltaY = (i32Y2 > i32Y1) ? (i32Y2 - i32Y1) : (i32Y1 - i32Y2);

    // Initialize the error term to negative half the X delta.
    i32Error = -i32DeltaX / 2;

    // Determine the direction to step in the Y axis when required.
    if(i32Y1 < i32Y2)
    {
        i32YStep = 1;
    }
    else
    {
        i32YStep = -1;
    }

    // Loop through all the points along the X axis of the line.
    for(; i32X1 <= i32X2; i32X1++)
    {
        // See if this is a steep line.
        if(bSteep)
        {
            // Plot this point of the line, swapping the X and Y coordinates.
            image_memory->values[i32Y1][i32X1] = color;
        }
        else
        {
            // Plot this point of the line, using the coordinates as is.
            image_memory->values[i32X1][i32Y1] = color;
        }
		
        // Increment the error term by the Y delta.
        i32Error += i32DeltaY;

        // See if the error term is now greater than zero.
        if(i32Error > 0)
        {
            // Take a step in the Y axis.
            i32Y1 += i32YStep;

            // Decrement the error term by the X delta.
            i32Error -= i32DeltaX;
        }
    }
}

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction, Scenario* scene) {
	switch (runway_direction) {
		case straight:
			draw_line(image_memory, RUNWAY_LEFT_START_X_POS, RUNWAY_START_Y_POS, RUNWAY_END_X_POS, RUNWAY_END_Y_POS -1, scene->runway);
			draw_line(image_memory, RUNWAY_RIGHT_START_X_POS, RUNWAY_START_Y_POS, RUNWAY_END_X_POS, RUNWAY_END_Y_POS -1, scene->runway);
		break;

		case right:
			draw_bezier_curve(image_memory, scene->runway, 
			                  118, 20, //Bottom
		                      70, 55, //Curve middle
			                  110, 90); //Top
		
			draw_bezier_curve(image_memory, scene->runway, 
			                  10, 20, //Bottom
		                      25, 90, //Curve middle
			                  110, 90); //Top
		break;

		case middle_left:
		break;

		case left:
			draw_bezier_curve(image_memory, scene->runway, 
			                  10, 20, //Bottom
		                      50, 55, //Curve middle
			                  18, 90); //Top
		
			draw_bezier_curve(image_memory, scene->runway, 
			                  118, 20, //Bottom
		                      103, 90, //Curve middle
			                  18, 90); //Top
		break;

		case middle_right:
		break;
	}
}

void update_display(Image_matrix* image_memory, Image_matrix* image_display, tContext sContext) {
	int line, column;
	
	invert_image_axis_y(image_memory);
	for (line = 0; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			if (image_display->values[line][column] != image_memory->values[line][column]) {
				image_display->values[line][column] = image_memory->values[line][column];
				GrContextForegroundSet(&sContext, image_display->values[line][column]);
				GrPixelDraw(&sContext, line, column);
			}
		}
	}
}

void update_console(Console *console, tContext sContext) {
	char buffer[10];
	
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	intToString(console->distance, buffer, 10, 10, 5);
	GrStringDraw(&sContext, buffer, -1, 10, (sContext.psFont->ui8Height+5)*9, true);
	
	intToString(console->race_lap, buffer, 10, 10, 2);
	GrStringDraw(&sContext, buffer, -1, 50, (sContext.psFont->ui8Height+5)*9, true);
	
	intToString(console->player_position, buffer, 10, 10, 3);
	GrStringDraw(&sContext, buffer, -1, 90, (sContext.psFont->ui8Height+5)*9, true);
}
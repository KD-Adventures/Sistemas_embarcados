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

void draw_car (Image_matrix* image_memory, const Car *car, Scenario* scene) {
	
	uint32_t car_x_position;
	car_x_position = car->runway_x_position + RUNWAY_RIGHT_START_X_POS; //- car->image->width/2;
	draw_image(image_memory, car->image, car_x_position, car->runway_y_position, car->color, false, 150);
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

void draw_arc(Image_matrix* image_memory, int32_t i32X, int32_t i32Y, int32_t i32Radius, int color) {
    int_fast32_t i32A, i32B, i32D, i32X1, i32Y1;

	int x_left_limit, x_right_limit, y_limit;
	x_left_limit = RUNWAY_LEFT_END_X_POS;
	x_right_limit = RUNWAY_RIGHT_END_X_POS;
	y_limit = HORIZON_Y_POSITION;
	
    // Initialize the variables that control the Bresenham circle drawing algorithm.
    i32A = 0;
    i32B = i32Radius;
    i32D = 3 - (2 * i32Radius);

    // Loop until the A delta is greater than the B delta, meaning that the
    // entire circle has been drawn.
    while(i32A <= i32B)
    {
        // Determine the row when subtracting the A delta.
        i32Y1 = i32Y - i32A;

        // See if this row is within the clipping region.
        if((i32Y1 >= MENU_HEIGHT) && (i32Y1 <= y_limit))
        {
            // Determine the column when subtracting the B delta.
            i32X1 = i32X - i32B;

            // If this column is within the clipping region, then draw a pixel
            // at that position.
			if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
			   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))
            {
                image_memory->values[i32X1][i32Y1] = color;
            }

            // Determine the column when adding the B delta.
            i32X1 = i32X + i32B;

            // If this column is within the clipping region, then draw a pixel
            // at that position.
			if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
			   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))

            {
                image_memory->values[i32X1][i32Y1] = color;
            }
        }

        // Determine the row when adding the A delta.
        i32Y1 = i32Y + i32A;

        // See if this row is within the clipping region, and the A delta is
        // not zero (otherwise, it will be the same row as when the A delta was
        // subtracted).
        if((i32Y1 >= MENU_HEIGHT) && (i32Y1 <= y_limit) && (i32A != 0))
        {
            // Determine the column when subtracting the B delta.
            i32X1 = i32X - i32B;

            // If this column is within the clipping region, then draw a pixel
            // at that position.
			if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
			   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))

            {
                image_memory->values[i32X1][i32Y1] = color;
            }

            // Determine the column when adding the B delta.
            i32X1 = i32X + i32B;

            // If this column is within the clipping region, then draw a pixel
            // at that position.
			if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
			   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))

            {
                image_memory->values[i32X1][i32Y1] = color;
            }
        }

        // Only draw the complementary pixels if the A and B deltas are
        // different (otherwise, they describe the same set of pixels).
        if(i32A != i32B)
        {
            // Determine the row when subtracting the B delta.
            //
            i32Y1 = i32Y - i32B;

            //
            // See if this row is within the clipping region.
            //
            if((i32Y1 >= MENU_HEIGHT) && (i32Y1 <= y_limit))
            {
                //
                // Determine the column when subtracting the a delta.
                //
                i32X1 = i32X - i32A;

                //
                // If this column is within the clipping region, then draw a
                // pixel at that position.
                //
				if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
				   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))
                {
                    image_memory->values[i32X1][i32Y1] = color;
                }

                //
                // Only draw the mirrored pixel if the A delta is non-zero
                // (otherwise, it will be the same pixel).
                //
                if(i32A != 0)
                {
                    //
                    // Determine the column when adding the A delta.
                    //
                    i32X1 = i32X + i32A;

                    //
                    // If this column is within the clipping region, then draw
                    // a pixel at that position.
                    //
					if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
					   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))
                    {
                        image_memory->values[i32X1][i32Y1] = color;
                    }
                }
            }

            //
            // Determine the row when adding the B delta.
            //
            i32Y1 = i32Y + i32B;

            //
            // See if this row is within the clipping region.
            //
            if((i32Y1 >= MENU_HEIGHT) && (i32Y1 <= y_limit))
            {
                //
                // Determine the column when subtracting the A delta.
                //
                i32X1 = i32X - i32A;

                //
                // If this column is within the clipping region, then draw a
                // pixel at that position.
                //
				if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
				   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))
                {
                    image_memory->values[i32X1][i32Y1] = color;
                }

                //
                // Only draw the mirrored pixel if the A delta is non-zero
                // (otherwise, it will be the same pixel).
                //
                if(i32A != 0)
                {
                    //
                    // Determine the column when adding the A delta.
                    //
                    i32X1 = i32X + i32A;

                    //
                    // If this column is within the clipping region, then draw
                    // a pixel at that position.
                    //
					if(((i32Y1 > Y_HALF_DISPLAY) && (i32X1 >= x_left_limit) && (i32X1 <= x_right_limit)) || 
					   ((i32Y1 <= Y_HALF_DISPLAY) && (i32X1 >= 0) && (i32X1 < DISPLAY_WIDTH)))
                    {
                        image_memory->values[i32X1][i32Y1] = color;
                    }
                }
            }
        }

        //
        // See if the error term is negative.
        //
        if(i32D < 0)
        {
            //
            // Since the error term is negative, adjust it based on a move in
            // only the A delta.
            //
            i32D += (4 * i32A) + 6;
        }
        else
        {
            //
            // Since the error term is non-negative, adjust it based on a move
            // in both the A and B deltas.
            //
            i32D += (4 * (i32A - i32B)) + 10;

            //
            // Decrement the B delta.
            //
            i32B -= 1;
        }

        //
        // Increment the A delta.
        //
        i32A++;
    }
}

void draw_runway(Image_matrix* image_memory, enum Runway_direction runway_direction, Scenario* scene) {
	switch (runway_direction) {
		case straight:
			draw_line(image_memory, RUNWAY_LEFT_START_X_POS, RUNWAY_START_Y_POS, RUNWAY_END_X_POS, RUNWAY_END_Y_POS -1, scene->runway);
			draw_line(image_memory, RUNWAY_RIGHT_START_X_POS, RUNWAY_START_Y_POS, RUNWAY_END_X_POS, RUNWAY_END_Y_POS -1, scene->runway);
		break;

		case left:
			draw_arc(image_memory, 102, -4, 94, scene->runway);
			draw_arc(image_memory, 178, 62, 72, scene->runway);
		break;

		case middle_left:
		break;

		case right:
			draw_arc(image_memory, 26, -4, 94, scene->runway);
			draw_arc(image_memory, -50, 62, 72, scene->runway);
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
	GrStringDraw(&sContext, "vol", -1, 50, (sContext.psFont->ui8Height+5)*9, true);
	GrStringDraw(&sContext, "pos", -1, 90, (sContext.psFont->ui8Height+5)*9, true);
	
	intToString(console->distance, buffer, 10, 10, 5);
	GrStringDraw(&sContext, buffer, -1, 10, (sContext.psFont->ui8Height+5)*9, true);
}
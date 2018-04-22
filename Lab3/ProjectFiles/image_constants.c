#include "image_constants.h"
#include "car_array.h"
#include "mountain_1_array.h"
#include "console_array.h"

Image CAR_BIG = {1,1,NULL};
Image MOUNTAIN_1 = {1,1,NULL};
Image CONSOLE = {1,1,NULL};
	
void load_images(){
	CAR_BIG.height = car_big_height;
	CAR_BIG.width = car_big_width;
	CAR_BIG.array = car_big_array;
	
	MOUNTAIN_1.array = mountain_1_array;
	MOUNTAIN_1.height = mountain_1_height;
	MOUNTAIN_1.width = mountain_1_width;
	
	CONSOLE.array = console_array;
	CONSOLE.height = console_height;
	CONSOLE.width = console_width;
}
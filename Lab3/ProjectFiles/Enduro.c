/*============================================================================
 *                    LAB 3 - Sistemas Embarcados - S72                      *
 *  Autores: Davi Boberg
 *           Lucas Kaminski de Freitas
 *---------------------------------------------------------------------------*
 * Autores das bibliotecas:
 *      Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 *      Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 *      Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 *      Mariana Carriao        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "TM4C129.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "grlib/grlib.h"
#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"
#include "graphic_functions.h"
#include "datatypes.h"
#include "image_constants.h"
#include "Mountain.h"
#include "Car.h"
#include "Console.h"
#include "utils.h"

// Globais no momento
enum Weather weather = DAY;
Scenario scenario;
enum Runway_direction runway_direction = straight;
int line, column;

tContext sContext;

Image_matrix* image_memory;
Image_matrix* image_display;

Car *car_player, *car_enemy;
Mountain* mountain;
Console* console;

bool move_left = false;
bool move_right = false;
bool accelerating = false;

int count_change_weather = 0;
int count_change_direction = 0;
int max_count_change_weather = 50;
int current_weather = 0;

char buffer[10];
	
	
/*===========================================================================*
 *                                 THREADS                                   *
 *===========================================================================*/
//void user_input (void const *args){
void user_input (void){
	int x = 0, y = 0;
	
	//joy_init();
	//button_init();
	
	move_left = false;
	move_right = false;

	x = joy_read_x();
	if(x < 1500)
		move_left = true;
	else if(x > 2500)
		move_right = true;
	
	y = joy_read_y();
	accelerating = (y > 2500);
}
osThreadDef (user_input, osPriorityNormal, 1, 0);

int set_current_weather(int new_weather){
switch (new_weather) {
		case 0:
			weather = DAY;
		break;

		case 1:
			weather = NIGHT;
		break;

		case 2:
			weather = FOG;
		break;

		case 3:
			weather = SUNRISE;
		break;

		case 4:
			weather = SUNSET;
		break;
		
		case 5:
			weather = SNOW;
		break;

		default:
			weather = DAY;
		break;
	}
}

/*===========================================================================*/
//void game_manager (void const *args){
void game_manager (void){	
	console->distance++;
	
	count_change_weather++;
	if (count_change_weather == max_count_change_weather)
	{
		count_change_weather = 0;
		current_weather++;
		if(current_weather == 6)
			current_weather = 0;
		set_current_weather(current_weather);
	}
	
	count_change_direction++;
	if (count_change_direction > (max_count_change_weather - 5))
	{
		count_change_direction = 0;
		if(runway_direction == left)
			runway_direction = straight;
		else if(runway_direction == straight)
			runway_direction = right;
		else if(runway_direction == right)
			runway_direction = left;
	}
	
	switch (runway_direction) {
		case right:
			mountain->x_position++;
		break;
		case left:
			mountain->x_position--;
		break;
		default:
		break;
	}
		
	if (move_left && (car_player->x_position < (RUNWAY_LEFT_START_X_POS - car_player->image->width)))
		car_player->x_position++;
	else if (move_right && (car_player->x_position > RUNWAY_RIGHT_START_X_POS))
		car_player->x_position--;
	
	if (accelerating && (car_player->y_position < 30))
		car_player->y_position++;
	else if (car_player->y_position > MENU_HEIGHT)
		car_player->y_position--;
	
	console->distance++;
}
//osThreadDef (game_manager, osPriorityNormal, 1, 0);


/*===========================================================================*/
void enemy_vehicles (void const *args){

}
osThreadDef (enemy_vehicles, osPriorityNormal, 1, 0);


/*===========================================================================*/
void player_vehicle (void const *args){

}
osThreadDef (player_vehicle, osPriorityNormal, 1, 0);


/*===========================================================================*/
void trajectory_manager (void const *args){

}
osThreadDef (trajectory_manager, osPriorityNormal, 1, 0);


/*===========================================================================*/
void game_stats (void const *args){

}
osThreadDef (game_stats, osPriorityNormal, 1, 0);


/*===========================================================================*/
void collision_detection (void const *args){

}
osThreadDef (collision_detection, osPriorityNormal, 1, 0);


/*===========================================================================*/
//void graphics (void const *args){
void graphics (void){
	int line, column;
	//Car* car_player; 
	//Mountain* mountain;
	//Console* console;

	//tContext sContext;
	int i = 0;

	//Image_matrix* image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	//Image_matrix* image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);

	//load_images();
	
	
	//Configuring the display.
	//This will be moved to user_output eventually

	//cfaf128x128x16Init();
	//GrContextInit(&sContext, &g_sCfaf128x128x16);
	//GrFlush(&sContext);
	//-------------

	clear_image(image_memory);
	
	set_weather(weather, &scenario);
	draw_background(image_memory, &scenario);


	draw_runway(image_memory, runway_direction, &scenario);

	draw_car(image_memory, car_player, &scenario);
	draw_car(image_memory, car_enemy, &scenario);
	draw_mountain(image_memory, mountain, &scenario);
	draw_console(image_memory, console);
	
	
	
	
	
	//Draw in the display pixels that might have changed.
	//This will be moved to user_output eventually
	invert_image_axis_x(image_memory);
	invert_image_axis_y(image_memory);
	//clear_image(image_display);
	for (line = 0; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			if (image_display->values[line][column] != image_memory->values[line][column]) {
				image_display->values[line][column] = image_memory->values[line][column];
				GrContextForegroundSet(&sContext, image_display->values[line][column]);
				GrPixelDraw(&sContext, line, column);
			}
		}
	}
	//delete_matrix_image(&image_memory);
	//delete_matrix_image(&image_display);
	
	//isso vai dentro do console
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	GrStringDraw(&sContext, "vol", -1, 50, (sContext.psFont->ui8Height+5)*9, true);
	GrStringDraw(&sContext, "pos", -1, 90, (sContext.psFont->ui8Height+5)*9, true);
	
	intToString(console->distance, buffer, 10, 10, 5);
	GrStringDraw(&sContext, buffer, -1, 10, (sContext.psFont->ui8Height+5)*9, true);
	//-------------
}
//osThreadDef (graphics, osPriorityNormal, 1, 800);


/*===========================================================================*/
void user_output (void const *args){

}
osThreadDef (user_output, osPriorityNormal, 1, 0);

void init_temp(){

	image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);

	load_images();

	cfaf128x128x16Init();
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	
	car_player = new_car(64, GROUND_Y_POSITION, 1, ClrYellow, 2);
	car_enemy = new_car(58, GROUND_Y_POSITION + 20, 1, ClrAquamarine, 1);
	mountain = new_mountain(40, ClrWhite, 1);
	console = new_console(0, MENU_Y_POSITION);
	
	joy_init();
	button_init();
}

int main(void) {
	osKernelInitialize();
	/*osThreadCreate(osThread(user_input), NULL);
	osThreadCreate(osThread(game_manager), NULL);
	osThreadCreate(osThread(enemy_vehicles), NULL);
	osThreadCreate(osThread(player_vehicle), NULL);
	osThreadCreate(osThread(trajectory_manager), NULL);
	osThreadCreate(osThread(game_stats), NULL);
	osThreadCreate(osThread(collision_detection), NULL);*/
	//osThreadCreate(osThread(graphics), NULL);
	//osThreadCreate(osThread(user_output), NULL);
	osKernelStart();

	//osDelay(osWaitForever);
	init_temp();
	while(1) {
		user_input();
		game_manager();
		graphics();
		osDelay(100);
	}
}

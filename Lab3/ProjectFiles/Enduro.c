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

/*===========================================================================*
 *                                 THREADS                                   *
 *===========================================================================*/
void user_input (void const *args){
	joy_init(); 
	button_init();
}
osThreadDef (user_input, osPriorityNormal, 1, 0);


/*===========================================================================*/
void game_manager (void const *args){

}
osThreadDef (game_manager, osPriorityNormal, 1, 0);


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
	enum Weather weather = SUNRISE;
	enum Runway_direction runway_direction = straight;
	int line, column;
	
	tContext sContext;
	int i = 0;
	
	Image_matrix* image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	Image_matrix* image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	
	//Configuring the display.
	//This will be moved to user_output eventually
	
	cfaf128x128x16Init();
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	//-------------
	

	clear_image(image_memory);
	draw_background(image_memory, weather);
	
	
	draw_runway(image_memory, runway_direction);
	
	//Draw in the display pixels that might have changed.
	//This will be moved to user_output eventually
	clear_image(image_display);

	for (line = 0; line < DISPLAY_HEIGHT; line++) {
		for (column = 0; column < DISPLAY_WIDTH; column++) {
			if (image_display->values[line][column] != image_memory->values[line][column]) {
				image_display->values[line][column] = image_memory->values[line][column];
				GrContextForegroundSet(&sContext, image_display->values[line][column]);
				GrPixelDraw(&sContext, line, column);
			}
		}
	}
	//-------------
	
	
	
	//delete_matrix_image(image_memory);
	//delete_matrix_image(image_display);
}
//osThreadDef (graphics, osPriorityNormal, 1, 800);


/*===========================================================================*/
void user_output (void const *args){
	
}
osThreadDef (user_output, osPriorityNormal, 1, 0);


int main(void) {
	//osKernelInitialize();
	/*osThreadCreate(osThread(user_input), NULL);
	osThreadCreate(osThread(game_manager), NULL);
	osThreadCreate(osThread(enemy_vehicles), NULL);
	osThreadCreate(osThread(player_vehicle), NULL);
	osThreadCreate(osThread(trajectory_manager), NULL);
	osThreadCreate(osThread(game_stats), NULL);
	osThreadCreate(osThread(collision_detection), NULL);*/
	//osThreadCreate(osThread(graphics), NULL);
	//osThreadCreate(osThread(user_output), NULL);
	//osKernelStart();
	
	//osDelay(osWaitForever);
	
	graphics();
}

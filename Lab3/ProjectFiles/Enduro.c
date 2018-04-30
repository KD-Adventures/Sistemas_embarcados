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

#define SIGNAL_CAR_LEFT 0x0001
#define SIGNAL_CAR_RIGHT 0x0002
#define SIGNAL_CAR_ACCEL 0x0004
	
	
/*===========================================================================*
 *                                 THREADS                                   *
 *===========================================================================*/
//Declarações
void user_input (void const *args);
void game_manager (void const *args);
void enemy_vehicles (void const *args);
void player_vehicle (void const *args);
void trajectory_manager (void const *args);
void game_stats (void const *args);
void collision_detection (void const *args);
void graphics (void const *args);
void user_output (void const *args);
osThreadDef (user_input, osPriorityNormal, 1, 0);
osThreadDef (game_manager, osPriorityNormal, 1, 0);
osThreadDef (enemy_vehicles, osPriorityNormal, 1, 0);
osThreadDef (player_vehicle, osPriorityNormal, 1, 0);
osThreadDef (trajectory_manager, osPriorityNormal, 1, 0);
osThreadDef (game_stats, osPriorityNormal, 1, 0);
osThreadDef (collision_detection, osPriorityNormal, 1, 0);
osThreadDef (graphics, osPriorityNormal, 1, 0);
osThreadDef (user_output, osPriorityNormal, 1, 0);

osMessageQDef(GameStateMsgBox, 16, &GameState);
osMessageQId GameStateMsgBox_Id;
osMessageQDef(ImageMatrixMsgBox, 16, &ImageMatrix);
osMessageQId ImageMatrixMsgBox_Id;

osMutexDef (MutexImageMemory);
osMutexId MutexImageMemory_Id;
/*===========================================================================*/
void user_input (void const *args){
	int x = 0;
	const osThreadId game_manager_id = (const osThreadId) args;
	
	while (1) {
		osSignalClear(game_manager_id, SIGNAL_CAR_LEFT | SIGNAL_CAR_RIGHT);

		x = joy_read_x();
		if (x < 1500)
			osSignalSet(game_manager_id, SIGNAL_CAR_LEFT);
		else if (x > 2500)
			osSignalSet(game_manager_id, SIGNAL_CAR_RIGHT);

		if (joy_read_y() > 2500)
			osSignalSet(game_manager_id, SIGNAL_CAR_ACCEL);
		else
			osSignalClear(game_manager_id, SIGNAL_CAR_ACCEL);
	}
}

/*===========================================================================*/
int set_current_weather(GameState *game, int new_weather){
	switch (new_weather) {
		case 0:
			game->weather = DAY;
		break;

		case 1:
			game->weather = NIGHT;
		break;
		
		case 5:
			game->weather = SNOW;
		break;

		default:
			game->weather = DAY;
		break;
	}
}

void game_manager (void const *args){
	GameState *game;
	int count_change_weather = 0;
	int count_change_direction = 0;
	int max_count_change_weather = 50;
	int current_weather = 0;
	bool car_move_left = false, car_move_right = false, car_accel = false; 
	osEvent event;

	game = (GameState*) malloc (sizeof(GameState));
	game->player_car = new_car(64, GROUND_Y_POSITION, 1, ClrYellow, 2);
	game->enemy_car = new_car(58, GROUND_Y_POSITION + 20, 1, ClrAquamarine, 1);
	game->mountain = new_mountain(40, ClrWhite, 1);
	game->console = new_console(0, MENU_Y_POSITION);
	game->runway_direction = straight;
	game->weather = DAY;
	
	osMessagePut(GameStateMsgBox_Id, (uint32_t) game, osWaitForever);
	
	while (1) {
		car_move_left = false;
		car_move_right = false;
		car_accel = false;
		
		event = osSignalWait(0, 100);
		if (event.status == osEventSignal) {
			car_accel = ((event.value.signals & SIGNAL_CAR_ACCEL) > 0);

			if ((event.value.signals & SIGNAL_CAR_LEFT) > 0)
				car_move_left = true;
			else if ((event.value.signals & SIGNAL_CAR_RIGHT) > 0)
				car_move_right = true;
		}
		
		game->console->distance++;
		
		count_change_weather++;
		if (count_change_weather == max_count_change_weather)
		{
			count_change_weather = 0;
			current_weather++;
			if(current_weather == 6)
				current_weather = 0;
			set_current_weather(game, current_weather);
		}
		
		count_change_direction++;
		if (count_change_direction > (max_count_change_weather - 5))
		{
			count_change_direction = 0;
			if(game->runway_direction == left)
				game->runway_direction = straight;
			else if(game->runway_direction == straight)
				game->runway_direction = right;
			else if(game->runway_direction == right)
				game->runway_direction = left;
		}
		
		switch (game->runway_direction) {
			case right:
				game->mountain->x_position++;
			break;
			case left:
				game->mountain->x_position--;
			break;
			default:
			break;
		}
			
		if (car_move_left && (game->player_car->x_position < (RUNWAY_LEFT_START_X_POS - game->player_car->image->width)))
			game->player_car->x_position++;
		else if (car_move_right && (game->player_car->x_position > RUNWAY_RIGHT_START_X_POS))
			game->player_car->x_position--;
		
		if (car_accel && (game->player_car->y_position < 30))
			game->player_car->y_position++;
		else if (game->player_car->y_position > MENU_HEIGHT)
			game->player_car->y_position--;
		
		game->console->distance++;
	}
}

/*===========================================================================*/
void enemy_vehicles (void const *args){

}

/*===========================================================================*/
void player_vehicle (void const *args){

}

/*===========================================================================*/
void trajectory_manager (void const *args){

}

/*===========================================================================*/
void game_stats (void const *args){

}


/*===========================================================================*/
void collision_detection (void const *args){

}

/*===========================================================================*/
void graphics (void const *args){
	Image_matrix* image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	GameState *game;
	Scenario scenario;
	osEvent event;
	
	osMessagePut(ImageMatrixMsgBox_Id, (uint32_t) image_memory, osWaitForever);
	
	while (1) {
		event = osMessageGet(GameStateMsgBox_Id, osWaitForever);
		if (event.status == osEventMessage) {
			game = (GameState*) event.value.p;
			break;
		}
	}
	
	clear_image(image_memory);
	while (1) {
		set_weather(game->weather, &scenario);
		
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		draw_background(image_memory, &scenario);
		draw_runway(image_memory, game->runway_direction, &scenario);
		draw_car(image_memory, game->player_car, &scenario);
		draw_car(image_memory, game->enemy_car, &scenario);
		draw_mountain(image_memory, game->mountain, &scenario);
		draw_console(image_memory, game->console);
		osMutexRelease(MutexImageMemory_Id);
	}
}

/*===========================================================================*/
void user_output (void const *args){
	tContext sContext;
	Image_matrix* image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	Image_matrix* image_memory;
	osEvent event;

	while(1) {
		event = osMessageGet(ImageMatrixMsgBox_Id, osWaitForever);
		if (event.status == osEventMessage) {
			image_memory = (Image_matrix*) event.value.p;
			break;
		}
	}
	
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	
	while(1) {
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		update_display(image_memory, image_display, sContext);
		osMutexRelease(MutexImageMemory_Id);
		//update_console(console, sContext);
		osDelay(100);
	}
}

/*===========================================================================*/
int main(void) {
	osThreadId game_manager_id;
	
	cfaf128x128x16Init();
	joy_init();
	button_init();
	osKernelInitialize();
	
	//Message boxes
	GameStateMsgBox_Id = osMessageCreate(osMessageQ(GameStateMsgBox), NULL);
	ImageMatrixMsgBox_Id = osMessageCreate(osMessageQ(ImageMatrixMsgBox), NULL);

	//Mutexes
	MutexImageMemory_Id = osMutexCreate(osMutex(MutexImageMemory));
	
	//Threads
	game_manager_id = osThreadCreate(osThread(game_manager), NULL);
	osThreadCreate(osThread(user_input), game_manager_id);
	/*osThreadCreate(osThread(enemy_vehicles), NULL);
	osThreadCreate(osThread(player_vehicle), NULL);
	osThreadCreate(osThread(trajectory_manager), NULL);
	osThreadCreate(osThread(game_stats), NULL);
	osThreadCreate(osThread(collision_detection), NULL);*/
	osThreadCreate(osThread(graphics), NULL);
	osThreadCreate(osThread(user_output), NULL);
	
	osKernelStart();

	osDelay(osWaitForever);
}

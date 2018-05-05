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

#define WEATHER_DURATION 50
#define RUNWAY_DIRECTION_DURATION 30 // TODO mudar para rand()
	
// Variaveis globais 

GameState *game;
Image_matrix* image_memory;

enum User_input_direction user_input_direction;
bool user_input_accel;


/*===========================================================================*
 *                                 THREADS                                   *
 *===========================================================================*/
//Declarações
void timer_game_frames (void const *args);
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

osThreadId user_input_id;
osThreadId game_manager_id;
osThreadId enemy_vehicles_id;
osThreadId player_vehicles_id;
osThreadId trajectory_manager_id;
osThreadId game_stats_id;
osThreadId collision_detection_id;
osThreadId graphics_id;
osThreadId user_output_id;

osTimerDef(timer_game, timer_game_frames);

osTimerId timer_game_id;

osMutexDef (MutexImageMemory);
osMutexId MutexImageMemory_Id;
/*===========================================================================*/

void timer_game_frames (void const *args){
	osSignalSet(user_input_id, 0x01);
}

int fputc(int c, FILE *f) {
  return(ITM_SendChar(c));
}

void user_input (void const *args){
	int x = 0;
	
	while (1) {
		
		osSignalWait(0x01, osWaitForever);
		
		user_input_direction = USER_INPUT_STRAIGHT;
		user_input_accel = false;
		
		user_input_direction = USER_INPUT_LEFT;
		user_input_accel  = false;
	
		x = joy_read_x();
		if (x < 1500) {
			user_input_direction = USER_INPUT_LEFT; 
		}
		else if (x > 2500) {
			user_input_direction = USER_INPUT_RIGHT; 
		}

		if (joy_read_y() > 2500) {
			user_input_accel  = true;
		}
		
		printf("User Input\n");
		
		osSignalSet(game_manager_id, 0x01);
	}
}

/*===========================================================================*/
int set_current_weather(WeatherManager* weather_manager){
	switch (weather_manager->weather_controller) {
		case 0:
			weather_manager->weather = DAY;
		break;

		case 1:
			weather_manager->weather = NIGHT;
		break;
		
		case 5:
			weather_manager->weather = SNOW;
		break;

		default:
			weather_manager->weather = DAY;
		break;
	}
}

void weather_manager (WeatherManager* weather_manager) {
	weather_manager->weather_timer++;
	if (weather_manager->weather_timer == weather_manager->weather_duration)
	{
		weather_manager->weather_timer = 0;
		weather_manager->weather_controller++;
		if (weather_manager->weather_controller == 6)
			weather_manager->weather_controller = 0;
		set_current_weather(weather_manager);
	}
}


void runway_manager (RunwayManager* runway_manager) {
	runway_manager->runway_direction_timer++;
	if (runway_manager->runway_direction_timer > runway_manager->runway_direction_duration)
	{
		runway_manager->runway_direction_timer = 0;
		runway_manager->runway_direction_duration = RUNWAY_DIRECTION_DURATION; // TODO gerar um rand
		
		// TODO Gerar um rand para mudar a direção
		if(runway_manager->runway_direction == left)
			runway_manager->runway_direction = straight;
		else if(runway_manager->runway_direction == straight)
			runway_manager->runway_direction = right;
		else if(runway_manager->runway_direction == right)
			runway_manager->runway_direction = left;
	}
}

void car_manager(Car* car) {
	
	if (car->direction == LEFT && (car->x_position < (RUNWAY_LEFT_START_X_POS - car->image->width)))
			car->x_position++;
		else if (car->direction == RIGHT && (car->x_position > RUNWAY_RIGHT_START_X_POS))
			car->x_position--;
		
		if (car->accelerating && (car->y_position < 30))
			car->y_position++;
		else if (car->y_position > MENU_HEIGHT)
			car->y_position--;
}

void game_manager (void const *args){
	int current_weather = 0;
	bool car_move_left = false, car_move_right = false, car_accel = false; 
	osEvent event;

	game = (GameState*) malloc (sizeof(GameState));
	game->player_car = new_car(64, GROUND_Y_POSITION, 1, ClrYellow, 2);
	game->enemy_car = new_car(58, GROUND_Y_POSITION + 20, 1, ClrAquamarine, 1);
	game->mountain = new_mountain(40, ClrWhite, 1);
	game->console = new_console(0, MENU_Y_POSITION);

	// Weather
	game->weather_manager.weather = DAY;
	game->weather_manager.weather_duration = WEATHER_DURATION;
	game->weather_manager.weather_controller = 0;
	game->weather_manager.weather_timer = 0;

	//Runaway direction
	game->runway_manager.runway_direction = straight;
	game->runway_manager.runway_direction_duration = RUNWAY_DIRECTION_DURATION;
	game->runway_manager.runway_direction_timer = 0;
	
	//osMessagePut(GameStateMsgBox_Id, (uint32_t) game, osWaitForever);
	
	while (1) {
		
		osSignalWait(0x01, osWaitForever);
		
		game->player_car->accelerating = user_input_accel;

		if (user_input_direction == USER_INPUT_LEFT)
			game->player_car->direction = LEFT;
		else if (user_input_direction == USER_INPUT_RIGHT)
			game->player_car->direction = RIGHT;
		
		game->console->distance++;
		weather_manager(&game->weather_manager);
		runway_manager(&game->runway_manager);
		
		switch (game->runway_manager.runway_direction) {
			case right:
				game->mountain->x_position++;
			break;
			case left:
				game->mountain->x_position--;
			break;
			default:
			break;
		}
			
		car_manager(game->player_car);
		
		game->console->distance++;
		
		printf("Game manager\n");
		
		osSignalSet(enemy_vehicles_id, 0x01);
		osSignalSet(player_vehicles_id, 0x01);
		osSignalSet(trajectory_manager_id, 0x01);	
	}
}

/*===========================================================================*/
void enemy_vehicles (void const *args){
	int previous_flags = 0;
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		printf("Enemy vehicles\n");
		previous_flags = osSignalSet(game_stats_id, 0x01);
		osSignalSet(collision_detection_id, 0x01);
	}
}

/*===========================================================================*/
void player_vehicle (void const *args){
	int previous_flags = 0;
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		printf("Player vehicles\n");
		previous_flags = osSignalSet(game_stats_id, 0x02);
		osSignalSet(collision_detection_id, 0x02);
	}
}

/*===========================================================================*/
void trajectory_manager (void const *args){
	int previous_flags = 0;
	while(1) {
		osSignalWait(0x0001, osWaitForever);
		
		printf("Trajectory manager\n");
		previous_flags = osSignalSet(game_stats_id, 0x04);
		osSignalSet(collision_detection_id, 0x04);
	}
}

/*===========================================================================*/
void game_stats (void const *args){
	while(1) {
		osSignalWait(0x07, osWaitForever);
		
		printf("Game stats\n");
		osSignalSet(graphics_id, 0x01);
	}
}


/*===========================================================================*/
void collision_detection (void const *args){
	while(1) {
		osSignalWait(0x07, osWaitForever);		
		printf("Collision detection\n");
	}
}

/*===========================================================================*/
void graphics (void const *args){
	Scenario scenario;
	image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	
	//osMessagePut(ImageMatrixMsgBox_Id, (uint32_t) image_memory, osWaitForever);
	
	while (1) {
		osSignalWait(0x01, osWaitForever);
	
		clear_image(image_memory);	
		set_weather(game->weather_manager.weather, &scenario);

		
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		draw_background(image_memory, &scenario);
		draw_runway(image_memory, game->runway_manager.runway_direction, &scenario);
		draw_car(image_memory, game->player_car, &scenario);
		draw_car(image_memory, game->enemy_car, &scenario);
		draw_mountain(image_memory, game->mountain, &scenario);
		draw_console(image_memory, game->console);
		osMutexRelease(MutexImageMemory_Id);
		
		printf("Graphics\n");
		osSignalSet(user_output_id, 0x01);
	}
}

/*===========================================================================*/
void user_output (void const *args){
	tContext sContext;
	Image_matrix* image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	
	while(1) {
		
		osSignalWait(0x01, osWaitForever);
		
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		update_display(image_memory, image_display, sContext);
		osMutexRelease(MutexImageMemory_Id);
		//update_console(console, sContext);
		
		printf("User output\n");
		osTimerStart(timer_game_id, 10);
	}
}

/*===========================================================================*/
int main(void) {	
	
	cfaf128x128x16Init();
	joy_init();
	button_init();
	osKernelInitialize();
	
	//Mutexes
	MutexImageMemory_Id = osMutexCreate(osMutex(MutexImageMemory));
	
	//Threads
	user_input_id = osThreadCreate(osThread(user_input), game_manager_id);
	game_manager_id = osThreadCreate(osThread(game_manager), NULL);
	

	enemy_vehicles_id = osThreadCreate(osThread(enemy_vehicles), NULL);
	player_vehicles_id = osThreadCreate(osThread(player_vehicle), NULL);
	trajectory_manager_id = osThreadCreate(osThread(trajectory_manager), NULL);
	game_stats_id = osThreadCreate(osThread(game_stats), NULL);
	collision_detection_id = osThreadCreate(osThread(collision_detection), NULL);

	graphics_id = osThreadCreate(osThread(graphics), NULL);
	user_output_id = osThreadCreate(osThread(user_output), NULL);
	
	timer_game_id = osTimerCreate(osTimer(timer_game), osTimerOnce, (void*)0);
	
	osTimerStart(timer_game_id, 10);
	
	osKernelStart();

	osDelay(osWaitForever);
}

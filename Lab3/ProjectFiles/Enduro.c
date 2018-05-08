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
#include "buzzer.h"
#include "graphic_functions.h"
#include <time.h>


#define WEATHER_DURATION 500
#define DAY_DURATION 1000
#define RACE_DAYS_DURATION 2

#define RUNWAY_WIDTH (RUNWAY_LEFT_START_X_POS - RUNWAY_RIGHT_START_X_POS)
#define COLLISION_FRONT_THRESHOLD 10

#define GAME_CLOCK_TIME 30

//#define SEM_PLACA_PARA_TESTAR
//#define BUZZER
	
// Variaveis globais 

GameState *game;

Image_matrix* image_memory;

tContext sContext;

enum User_input_direction user_input_direction;
bool user_input_accel;
bool user_input_break;
bool user_start_button_pressed;
bool user_end_button_pressed;
uint32_t x_distance;


/*===========================================================================*
 *                                 THREADS                                   *
 *===========================================================================*/
//Declarações
void timer_game_frames (void const *args);
void user_input (void const *args);
void menu (void const *args);
void game_manager (void const *args);
void enemy_vehicles (void const *args);
void player_vehicle (void const *args);
void trajectory_manager (void const *args);
void game_stats (void const *args);
void collision_detection (void const *args);
void graphics (void const *args);
void user_output (void const *args);
void start_game_clock_timer();
void delete_enemy_car (Car *enemy_car, int index);

osThreadDef (user_input, osPriorityNormal, 1, 0);
osThreadDef (menu, osPriorityNormal, 1, 0);
osThreadDef (game_manager, osPriorityNormal, 1, 0);
osThreadDef (enemy_vehicles, osPriorityNormal, 1, 0);
osThreadDef (player_vehicle, osPriorityNormal, 1, 0);
osThreadDef (trajectory_manager, osPriorityNormal, 1, 0);
osThreadDef (game_stats, osPriorityNormal, 1, 0);
osThreadDef (collision_detection, osPriorityNormal, 1, 0);
osThreadDef (graphics, osPriorityNormal, 1, 0);
osThreadDef (user_output, osPriorityNormal, 1, 0);

osThreadId user_input_id;
osThreadId menu_id;
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

osMutexDef(MutexConsole);
osMutexId MutexConsole_id;
/*===========================================================================*/

void timer_game_frames (void const *args){
	osSignalSet(user_input_id, 0x01);
}

int fputc(int c, FILE *f) {
  return(ITM_SendChar(c));
}

void start_game_clock_timer()
{
	osTimerStart(timer_game_id, GAME_CLOCK_TIME);
}

void user_input (void const *args){
	int x = 0;
	while (1) {
		
		osSignalWait(0x01, osWaitForever);
		if (game->player_car->collision_detected) {
			user_input_direction = USER_INPUT_STRAIGHT;
			user_input_accel = false;
			user_input_break = false;
			user_start_button_pressed = false;
			user_end_button_pressed = false;
		}
		else {
#ifdef SEM_PLACA_PARA_TESTAR
			user_input_direction = USER_INPUT_STRAIGHT;
			user_input_accel = false;
			user_input_break = false;
			user_start_button_pressed = false;
			user_end_button_pressed = false;
#else
			user_input_direction = USER_INPUT_STRAIGHT;
			user_input_accel = false;
			user_input_break = false;
		
			x = joy_read_x();
			if (x < 1500) {
				user_input_direction = USER_INPUT_RIGHT; 
			}
			else if (x > 2500) {
				user_input_direction = USER_INPUT_LEFT; 
			}
			
			user_input_accel  = (joy_read_y() > 2500);
			user_input_break = (joy_read_y() < 1500);
			user_start_button_pressed = button_read_s1();
			user_end_button_pressed = button_read_s2();
#endif
		}
		
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
		srand(osKernelSysTick());
		runway_manager->runway_direction_timer = 0;
		runway_manager->runway_direction_duration = 30 + (rand() / (RAND_MAX / 30));
		
		if (runway_manager->runway_direction == left || runway_manager->runway_direction == right)
			runway_manager->runway_direction = straight;
		else if ((rand() / (RAND_MAX / 2)) == 1)
			runway_manager->runway_direction = right;
		else
			runway_manager->runway_direction = left;
	}
}

void car_handle_colision(Car* car) {
	if (car->collision_detected) {
		//Decreases car speed
		if (car->speed > car->collision_correction_speed)
			car->speed -= car->collision_correction_speed;
		if (car->collision_correction_speed > 0)
			car->collision_correction_speed--;
		
		//Moves car
		switch(car->collision_correction_direction) {
			case COLLISION_RIGHT:
				car->runway_x_position += car->collision_correction_position;
			  if (car->collision_correction_position > 0)
					car->collision_correction_position--;
			break;
			case COLLISION_LEFT:
				car->runway_x_position -= car->collision_correction_position;
			  if (car->collision_correction_position > 0)
					car->collision_correction_position--;
			break;
			case COLLISION_FRONT:
				//Nothing here
				break;
			default:
				break;
		}
		
		if(car->collision_correction_position == 0 && car->collision_correction_speed == 0)
			car->collision_detected = false;		
	}
}

void car_manager(Car* car) {
	if (car->direction == LEFT)
		car->runway_x_position += 5;
	else if (car->direction == RIGHT)
		car->runway_x_position -= 5;
		
	if (car->accelerating && (car->runway_y_position < 30)) {
		car->runway_y_position++;
	}
	else if (car->runway_y_position > MENU_HEIGHT) {
		car->runway_y_position--;
	}
	
	if (car->accelerating && car->speed < car->max_speed)
	{
		car->speed++;
	}
	else if (car->breaking && car->speed > 1)
	{
		car->speed--;
	}
	
	car_handle_colision(car);
	car->runway_distance += car->speed;
	
	osMutexWait(MutexConsole_id, osWaitForever);
	game->console->distance = car->runway_distance;
	osMutexRelease(MutexConsole_id);
}

void game_time_manager(GameState *game) {
	game->day_time++;
	
	if (game->day_time >= (game->day_duration * (game->day + 1))) {
		game->day++;
	}
	
	if (game->day >= game->day_max) {
		game->state = GAME_OVER;
	}
	
	osMutexWait(MutexConsole_id, osWaitForever);
	game->console->race_lap = game->day + 1;
	osMutexRelease(MutexConsole_id);
}

void game_manager (void const *args){
	int i;
	int current_weather = 0;
	bool car_move_left = false, car_move_right = false, car_accel = false;
	
	game = (GameState*) malloc (sizeof(GameState));
	game->mountain = new_mountain(40, ClrWhite, 1);

	game->max_enemy_cars = MAX_ENEMY_CARS;
	game->enemy_cars_quantity = 0;
	
	game->state = GAME_OVER;
	
	//osMessagePut(GameStateMsgBox_Id, (uint32_t) game, osWaitForever);
	
	while (1) {	
		while (game->state == GAME_OVER) {
			osSignalWait(0x01, osWaitForever);
			
			if (user_start_button_pressed == true) {
				game->state = GAME_RUNNING;
				
				// Weather
				game->weather_manager.weather = DAY;
				game->weather_manager.weather_duration = WEATHER_DURATION;
				game->weather_manager.weather_controller = 0;
				game->weather_manager.weather_timer = 0;
				
				game->day = 0;
				game->day_duration = DAY_DURATION;
				game->day_max = RACE_DAYS_DURATION;
				game->day_time = 0;
				
				if (game->console != NULL) {
					game->console->race_lap = 1;
					game->console->player_position = 200;
					game->player_car->runway_distance = 0;
				}
			}
			//printf("Game Not Started\n");						
			osSignalSet(menu_id, 0x01);
		}
				
		while (game->state == GAME_RUNNING) {
			osSignalWait(0x01, osWaitForever);
			
			if (user_end_button_pressed == true) {
				game->state = GAME_OVER;
				clear_image(image_memory);
				
				for(i = 0; i < game->enemy_cars_quantity; i++)
					delete_enemy_car(game->enemy_car[i], i);
				
				osSignalSet(user_output_id, 0x01);
				break;
			}
					
			game_time_manager(game);
			
			//printf("Game Started\n");
			
			osSignalSet(enemy_vehicles_id, 0x01);
			osSignalSet(player_vehicles_id, 0x01);
			osSignalSet(trajectory_manager_id, 0x01);				
		}
	}
}

/*===========================================================================*/
void menu (void const *args){
	bool game_finished = false;
	
	while (1) {
		osSignalWait(0x01, osWaitForever);
		game_finished = (game->console->distance > 0);
		draw_menu(image_memory, sContext, game_finished, game->console);
		osSignalSet(user_output_id, 0x01);
	}
}


void delete_enemy_car (Car *enemy_car, int index) {
	int i;
	
	if (index < game->enemy_cars_quantity - 1) {
		for (i = index; i < game->enemy_cars_quantity - 1; i++) {
			game->enemy_car[i] = game->enemy_car[i+1];
		}
	}
	
	free(enemy_car);
	game->enemy_car[game->enemy_cars_quantity - 1] = NULL;
	game->enemy_cars_quantity--;
}

/*===========================================================================*/
void enemy_vehicles (void const *args){
	int i = 0;
	double random;
	uint32_t starting_x_position, starting_y_position;
	bool too_far_away = false;
	bool create_car;
	int player_position;
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		random = ((double) rand()) / ((double) RAND_MAX);
		if ((game->enemy_cars_quantity < game->max_enemy_cars) && (random < 0.2)) {
			osMutexWait(MutexConsole_id, osWaitForever);
			player_position = N_ENEMIES_GOAL - game->console->player_position;
			osMutexRelease(MutexConsole_id);
	
			create_car = true;
			starting_x_position = 33 + rand() / (RAND_MAX / 62);
			if (game->player_car->speed > MAX_CAR_SPEED / 2) {
				starting_y_position = GROUND_HEIGHT;				
				
				//Checks if x_position isn't already occupied
				for(i = 0; i < game->enemy_cars_quantity; i++) {
					if (difference(starting_x_position, game->enemy_car[i]->runway_x_position) <= game->enemy_car[i]->image->width)
						create_car = false; //Does not create the car in this case.
				}
			}
			else {
				starting_y_position = MENU_HEIGHT;
				
				//Checks if x_position isn't already occupied
				if (difference(starting_x_position, game->player_car->runway_x_position) <= game->player_car->image->width)
					create_car = false; //Does not create the car in this case.
				for(i = 0; i < game->enemy_cars_quantity; i++) {
					if (difference(starting_x_position, game->enemy_car[i]->runway_x_position) <= game->enemy_car[i]->image->width)
						create_car = false; //Does not create the car in this case.
				}
				
				if (create_car) {
					osMutexWait(MutexConsole_id, osWaitForever);
					if (game->console->player_position < N_ENEMIES_GOAL)
						game->console->player_position++;
					osMutexRelease(MutexConsole_id);
				}
			}
			
			if (create_car) {
				game->enemy_car[game->enemy_cars_quantity] = new_car(starting_x_position, starting_y_position, game->console->distance + 30, MAX_CAR_SPEED/2, ClrAquamarine, CAR_MEDIUM);
				game->enemy_car[game->enemy_cars_quantity]->direction = STRAIGHT;
				game->enemy_car[game->enemy_cars_quantity]->accelerating = false;
				game->enemy_car[game->enemy_cars_quantity]->breaking = false;
				game->enemy_cars_quantity++;
			}
		}
		
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			if (game->enemy_car[i]->runway_y_position < MENU_HEIGHT) {
				osMutexWait(MutexConsole_id, osWaitForever);
				if (game->console->player_position > 0)
					game->console->player_position--;
				else
					game->state = GAME_OVER;
				osMutexRelease(MutexConsole_id);
			}
				
			too_far_away = (game->enemy_car[i]->runway_y_position < MENU_HEIGHT || game->enemy_car[i]->runway_y_position > 80);
			if (game->enemy_cars_quantity > 0 && too_far_away)
				delete_enemy_car(game->enemy_car[i], i);
		}
		
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			if(game->player_car->speed > game->enemy_car[i]->speed)
				game->enemy_car[i]->runway_y_position -= game->player_car->speed - game->enemy_car[i]->speed;
			else
				game->enemy_car[i]->runway_y_position += game->enemy_car[i]->speed + game->player_car->speed;
			
			if (game->enemy_car[i]->runway_y_position < MENU_HEIGHT + 20)
				game->enemy_car[i]->image = switch_image((const int*) CAR_BIG_ARRAY, 11, 25, game->enemy_car[i]->image);
			else if (game->enemy_car[i]->runway_y_position < (MENU_HEIGHT + GROUND_HEIGHT - 20))
				game->enemy_car[i]->image = switch_image((const int*) CAR_MEDIUM_ARRAY, 9, 18, game->enemy_car[i]->image);
			else
				game->enemy_car[i]->image = switch_image((const int*) CAR_SMALL_ARRAY, 6, 10, game->enemy_car[i]->image);
		}
		
		//printf("Enemy vehicles\n");
		osSignalSet(game_stats_id, 0x01);
		osSignalSet(collision_detection_id, 0x01);
	}
}

/*===========================================================================*/
void player_vehicle (void const *args){
	int i = 0;
	game->player_car = new_car((uint32_t)(RUNWAY_WIDTH/2), GROUND_Y_POSITION, 0, 1, ClrYellow, CAR_BIG);
	
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		game->player_car->accelerating = user_input_accel;
		game->player_car->breaking = user_input_break;

		if (user_input_direction == USER_INPUT_LEFT)
			game->player_car->direction = LEFT;
		else if (user_input_direction == USER_INPUT_RIGHT)
			game->player_car->direction = RIGHT;
		else
			game->player_car->direction = STRAIGHT;
		
		car_manager(game->player_car);		

		//printf("Player vehicles\n");
		osSignalSet(game_stats_id, 0x02);
		osSignalSet(collision_detection_id, 0x02);
	}
}

/*===========================================================================*/
void trajectory_manager (void const *args){
	
	//Runway direction
	game->runway_manager.runway_direction = straight;
	game->runway_manager.runway_direction_timer = 0;
	game->runway_manager.runway_width = RUNWAY_WIDTH;
	
	while(1) {
		osSignalWait(0x0001, osWaitForever);
		
		weather_manager(&game->weather_manager);
		
		runway_manager(&game->runway_manager);
		
		switch (game->runway_manager.runway_direction) {
			case right:
				game->mountain->x_position--;
			break;
			case left:
				game->mountain->x_position++;
			break;
			default:
			break;
		}
		
		//printf("Trajectory manager\n");
		osSignalSet(game_stats_id, 0x04);
		osSignalSet(collision_detection_id, 0x04);
	}
}

/*===========================================================================*/
void game_stats (void const *args){
	
	osMutexWait(MutexConsole_id, osWaitForever);
	game->console = new_console(0, 0);
	osMutexRelease(MutexConsole_id);

	while(1) {
		osSignalWait(0x07, osWaitForever);

		osMutexWait(MutexConsole_id, osWaitForever);
		update_console(game->console, sContext);
		osMutexRelease(MutexConsole_id);
		//printf("Game stats\n");
		osSignalSet(graphics_id, 0x01);
	}
}

void collision_detection_runway(Car* car) {
	if (car->runway_x_position >= 108 - (car->image->width / 2)) {
		car->collision_correction_direction = COLLISION_LEFT;
		car->collision_correction_position = 2;
		car->collision_correction_speed = 0;
		car->collision_detected = true;
	}
	else if (car->runway_x_position < 20 + (car->image->width / 2)) {
		car->collision_correction_direction = COLLISION_RIGHT;
		car->collision_correction_position = 2;
		car->collision_correction_speed = 0;
		car->collision_detected = true;
	}
}

void collision_detection_enemies(Car* player_car, Car* enemy_car) {
	if (difference(enemy_car->runway_y_position, player_car->runway_y_position) < player_car->image->height) {
		x_distance = difference(player_car->runway_x_position, enemy_car->runway_x_position);
		
		//Collision - player car behind
		if (x_distance < COLLISION_FRONT_THRESHOLD) {
			player_car->collision_correction_direction = COLLISION_FRONT;
			player_car->collision_correction_position = 0;
			player_car->collision_correction_speed = 3;
			player_car->collision_detected = true;
		}
		
		//Collision - player car on the right
		else if ((player_car->runway_x_position > enemy_car->runway_x_position) && (x_distance < enemy_car->image->width)) {
			player_car->collision_correction_direction = COLLISION_RIGHT;
			player_car->collision_correction_position = 5;
			player_car->collision_correction_speed = 3;
			player_car->collision_detected = true;
		}
		
		//Collision - player car on the left
		else if ((player_car->runway_x_position < enemy_car->runway_x_position) && (x_distance < player_car->image->width)) {
			player_car->collision_correction_direction = COLLISION_LEFT;
			player_car->collision_correction_position = 5;
			player_car->collision_correction_speed = 3;
			player_car->collision_detected = true;
		}
	}
}

/*===========================================================================*/
void collision_detection (void const *args){
	int i = 0;
	while(1) {
		osSignalWait(0x07, osWaitForever);		
		//printf("Collision detection\n");
		
		collision_detection_runway(game->player_car);
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			collision_detection_enemies(game->player_car, game->enemy_car[i]);
		}
	}
}

/*===========================================================================*/
void graphics (void const *args){
	Scenario scenario;
	int i = 0;
	image_memory = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);
	
	//osMessagePut(ImageMatrixMsgBox_Id, (uint32_t) image_memory, osWaitForever);
	
	while (1) {
		osSignalWait(0x01, osWaitForever);
	
		clear_image(image_memory);	
		set_weather(game->weather_manager.weather, &scenario);

		draw_background(image_memory, &scenario);
		draw_runway(image_memory, game->runway_manager.runway_direction, &scenario);
		draw_car(image_memory, game->player_car, game->runway_manager.runway_direction);

		osMutexWait(MutexConsole_id, osWaitForever);
		draw_console(image_memory, game->console);
		osMutexRelease(MutexConsole_id);
		
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			draw_car(image_memory, game->enemy_car[i], game->runway_manager.runway_direction);
		}
		draw_mountain(image_memory, game->mountain, &scenario);
		
		//printf("Graphics\n");
		osSignalSet(user_output_id, 0x01);
	}
}

/*===========================================================================*/
void user_output (void const *args){
	int sound_period = 400; // nao sei qual é a unidade para o periodo do buzzer
	bool new_collision = false;
	Image_matrix* image_display = new_matrix_image(DISPLAY_HEIGHT, DISPLAY_WIDTH);

	while(1) {
		osSignalWait(0x01, osWaitForever);
		update_display(image_memory, image_display, sContext);
		
#ifdef BUZZER_HELL
		//Very simple sound manager to "see" if works
		if (game->player_car->accelerating) {
			sound_period--;
			buzzer_per_set(sound_period);
			buzzer_write(true);
		}
		else {
			sound_period = 400;
			buzzer_write(false);
		}
		
		if (new_collision == false && game->player_car->collision_detected) {
			buzzer_per_set(500);
			buzzer_write(true);
		}
#endif
		start_game_clock_timer();
	}
}

/*===========================================================================*/
int main(void) {	
	
#ifndef SEM_PLACA_PARA_TESTAR
	cfaf128x128x16Init();
	joy_init();
	button_init();
	buzzer_init();
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
#endif
	osKernelInitialize();
	
	// Mutex
	MutexConsole_id = osMutexCreate(osMutex(MutexConsole));
	
	//Threads
	user_input_id = osThreadCreate(osThread(user_input), game_manager_id);
	menu_id = osThreadCreate(osThread(menu), NULL);
	game_manager_id = osThreadCreate(osThread(game_manager), NULL);

	enemy_vehicles_id = osThreadCreate(osThread(enemy_vehicles), NULL);
	player_vehicles_id = osThreadCreate(osThread(player_vehicle), NULL);
	trajectory_manager_id = osThreadCreate(osThread(trajectory_manager), NULL);
	game_stats_id = osThreadCreate(osThread(game_stats), NULL);
	collision_detection_id = osThreadCreate(osThread(collision_detection), NULL);

	graphics_id = osThreadCreate(osThread(graphics), NULL);
	user_output_id = osThreadCreate(osThread(user_output), NULL);
	
	// Timer
	timer_game_id = osTimerCreate(osTimer(timer_game), osTimerOnce, (void*)0);
	
	osTimerStart(timer_game_id, 100);
	
	osKernelStart();

	osDelay(osWaitForever);
}

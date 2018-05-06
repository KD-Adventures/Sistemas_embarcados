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


#define WEATHER_DURATION 50
#define DAY_DURATION 100;
#define RACE_DAYS_DURATION 2;

#define RUNWAY_DIRECTION_DURATION 30 // TODO mudar para rand()
#define RUNWAY_WIDTH RUNWAY_RIGHT_START_X_POS - RUNWAY_LEFT_START_X_POS

#define MAX_ENEMY_CARS 1

#define GAME_CLOCK_TIME 15

#define SEM_PLACA_PARA_TESTAR
	
// Variaveis globais 

GameState *game;

Image_matrix* image_memory;

enum User_input_direction user_input_direction;
bool user_input_accel;
bool user_input_break;
bool user_start_button_pressed;


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
void start_game_clock_timer();

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

void start_game_clock_timer()
{
	osTimerStart(timer_game_id, GAME_CLOCK_TIME);
}

void user_input (void const *args){
	int x = 0;
	bool start_button  = false;
	while (1) {
		
		osSignalWait(0x01, osWaitForever);
		
#ifdef SEM_PLACA_PARA_TESTAR
		user_input_direction = USER_INPUT_STRAIGHT;
		user_input_accel = false;
		user_input_break = false;
		user_start_button_pressed = false;
#else
		user_input_direction = USER_INPUT_STRAIGHT;
		user_input_accel = false;
		user_input_break = false;
		user_start_button_pressed = false;
	
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
		else if (joy_read_y() < 1500) {
			user_input_break = true;
		}
		
		start_button = button_read_s1();
		if (start_button) {
			user_start_button_pressed = true;
		}
#endif
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

void car_handle_colision(Car* car) {
	if (car->collision_detected) {
		car->speed -= car->collision_correction_speed;
		if (car->speed < 1) {
			car->speed = 1;
			car->collision_correction_speed--;
		}
		
		switch(car->collision_correction_direction) {
			case COLLISION_RIGHT:
				car->runway_x_position += car->collision_correction_position;
				car->collision_correction_position--;
			break;
			case COLLISION_LEFT:
				car->runway_x_position -= car->collision_correction_position;
				car->collision_correction_position--;
			break;
			// TODO front nem tem
			case COLLISION_FRONT:
				break;
			default:
				break;
		}
		
		if(car->collision_correction_position == 0 && car->collision_correction_speed == 0)
			car->collision_detected = false;		
	}
}

void car_manager(Car* car) {
	
	if (car->direction == LEFT )
		car->runway_x_position++;
	else if (car->direction == RIGHT)
		car->runway_x_position--;
		
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
}

void game_time_manager(GameState *game) {
	game->day_time++;
	
	if (game->day_time >= game->day_duration) {
		game->day++;
	}
	
	if (game->day >= game->day_max) {
		game->state = GAME_OVER;
	}
}

void game_manager (void const *args){
	int current_weather = 0;
	bool car_move_left = false, car_move_right = false, car_accel = false; 

	game = (GameState*) malloc (sizeof(GameState));
	game->mountain = new_mountain(40, ClrWhite, 1);

	// Weather
	game->weather_manager.weather = DAY;
	game->weather_manager.weather_duration = WEATHER_DURATION;
	game->weather_manager.weather_controller = 0;
	game->weather_manager.weather_timer = 0;
	
	game->day = 0;
	game->day_duration = DAY_DURATION;
	game->day_max = RACE_DAYS_DURATION;
	game->day_time = 0;
	game->state = GAME_OVER;
	
	game->max_enemy_cars = MAX_ENEMY_CARS;
	game->enemy_cars_quantity = 0;
	
	//osMessagePut(GameStateMsgBox_Id, (uint32_t) game, osWaitForever);
	
	while (1) {
		
		while (game->state == GAME_OVER) {
			osSignalWait(0x01, osWaitForever);
			
			if (user_start_button_pressed == true) {
				game->state = GAME_RUNNING;
			}
			printf("Game Not Started\n");
			start_game_clock_timer();
		}
		
#ifndef SEM_PLACA_PARA_TESTAR
		while (user_start_button_pressed) {} 
#endif	
		
		while (game->state == GAME_RUNNING) {
			osSignalWait(0x01, osWaitForever);
			
			if (user_start_button_pressed == true) {
				game->state = GAME_OVER;
				continue;
			}
					
			game_time_manager(game);
			
			printf("Game Started\n");
			
			osSignalSet(enemy_vehicles_id, 0x01);
			osSignalSet(player_vehicles_id, 0x01);
			osSignalSet(trajectory_manager_id, 0x01);				
		}
	}
}

/*===========================================================================*/
void enemy_vehicles (void const *args){
	int i = 0;
	bool too_far_away = false;
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		if (game->enemy_cars_quantity < game->max_enemy_cars && rand() < 0.2) {
			// TODO Ajustar a distancia na pista
			// TODO ajustar GameState para suportar mais de 1 carro
			// TODO impedir que carros sejam criados atras do player
			game->enemy_cars_quantity++;
			game->enemy_car = new_car(RUNWAY_WIDTH - 30, GROUND_Y_POSITION + 20, game->console->distance + 30, MAX_CAR_SPEED/2, ClrAquamarine, 1);
			game->enemy_car->direction = STRAIGHT;
			game->enemy_car->accelerating = false;
			game->enemy_car->breaking = false;
		}
		else
		
		too_far_away = (game->enemy_car->runway_distance > game->console->distance + 50) || (game->enemy_car->runway_distance < game->console->distance - 20);
		if (game->enemy_cars_quantity > 0 && too_far_away) {
			free(game->enemy_car);
			game->enemy_cars_quantity--;
		}
		
		// TODO ajustar para mais de um carro
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			car_manager(game->player_car);
		}
		
		printf("Enemy vehicles\n");
		osSignalSet(game_stats_id, 0x01);
		osSignalSet(collision_detection_id, 0x01);
	}
}

/*===========================================================================*/
void player_vehicle (void const *args){
	game->player_car = new_car(RUNWAY_WIDTH/2, GROUND_Y_POSITION, 0, 1, ClrYellow, 2);
	
	while(1) {
		osSignalWait(0x01, osWaitForever);
		
		game->player_car->accelerating = user_input_accel;
		game->player_car->breaking = user_input_break;

		if (user_input_direction == USER_INPUT_LEFT)
			game->player_car->direction = LEFT;
		else if (user_input_direction == USER_INPUT_RIGHT)
			game->player_car->direction = RIGHT;
		
		car_manager(game->player_car);
		
		
		printf("Player vehicles\n");
		osSignalSet(game_stats_id, 0x02);
		osSignalSet(collision_detection_id, 0x02);
	}
}

/*===========================================================================*/
void trajectory_manager (void const *args){
	
	//Runaway direction
	game->runway_manager.runway_direction = straight;
	game->runway_manager.runway_direction_duration = RUNWAY_DIRECTION_DURATION;
	game->runway_manager.runway_direction_timer = 0;
	game->runway_manager.runway_width = RUNWAY_WIDTH;
	
	while(1) {
		osSignalWait(0x0001, osWaitForever);
		
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
		
		printf("Trajectory manager\n");
		osSignalSet(game_stats_id, 0x04);
		osSignalSet(collision_detection_id, 0x04);
	}
}

/*===========================================================================*/
void game_stats (void const *args){
	game->console = new_console(0, MENU_Y_POSITION);
	while(1) {
		osSignalWait(0x07, osWaitForever);
		
		// TODO usar mutex para o controle do painel de instrumentos (requisito)
		game->console->distance++;
		
		printf("Game stats\n");
		osSignalSet(graphics_id, 0x01);
	}
}

void collision_detection_runway(Car* car) {
	// Collision left
	if (car->runway_x_position >= (RUNWAY_WIDTH - car->image->width/2)) {
		car->collision_correction_direction = COLLISION_LEFT;
		car->collision_correction_position = 3;
		car->collision_correction_speed = 1;
		car->collision_detected = true;
	}
	else if (car->runway_x_position < car->image->width/2) {
		car->collision_correction_direction = COLLISION_LEFT;
		car->collision_correction_position = 3;
		car->collision_correction_speed = 1;
		car->collision_detected = true;
	}
}

void collision_detection_enemies(Car* player_car, Car* enemy_car) {

	if (enemy_car->runway_distance - player_car->runway_distance > 0 &&  enemy_car->runway_distance - enemy_car->runway_distance < player_car->image->height) {
		if (player_car->runway_x_position - player_car->image->width/2 < enemy_car->runway_x_position + enemy_car->image->width/2) {
			// colisao pela direita
			player_car->collision_correction_direction = COLLISION_RIGHT;
			player_car->collision_correction_position = 5;
			player_car->collision_correction_speed = 3;
			player_car->collision_detected = true;
		}
		else if (player_car->runway_x_position + player_car->image->width/2 < enemy_car->runway_x_position - enemy_car->image->width/2) {
			// colisao pela direita
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
		printf("Collision detection\n");
		
		collision_detection_runway(game->player_car);
		// TODO ajustar para suportar mais de um inimigo
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			collision_detection_enemies(game->player_car, game->enemy_car);
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

		
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		draw_background(image_memory, &scenario);
		draw_runway(image_memory, game->runway_manager.runway_direction, &scenario);
		draw_car(image_memory, game->player_car, &scenario);
		for(i = 0; i < game->enemy_cars_quantity; i++) {
			draw_car(image_memory, game->enemy_car, &scenario);
		}
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

#ifdef SEM_PLACA_PARA_TESTAR	
#else
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
#endif	
	while(1) {
		
		osSignalWait(0x01, osWaitForever);
		
#ifdef SEM_PLACA_PARA_TESTAR

#else
		osMutexWait(MutexImageMemory_Id, osWaitForever);
		update_display(image_memory, image_display, sContext);
		osMutexRelease(MutexImageMemory_Id);
#endif
		//update_console(console, sContext);
		
		printf("User output\n");
		start_game_clock_timer();
	}
}

/*===========================================================================*/
int main(void) {	
	
#ifdef SEM_PLACA_PARA_TESTAR
#else
	cfaf128x128x16Init();
	joy_init();
	button_init();
#endif
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
	
	osTimerStart(timer_game_id, 100);
	
	osKernelStart();

	osDelay(osWaitForever);
}

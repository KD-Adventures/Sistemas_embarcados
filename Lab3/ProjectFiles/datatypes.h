#ifndef DATATYPES
#define DATATYPES

#include <stdlib.h>
#include "Car.h"
#include "Console.h"
#include "Mountain.h"

#define MAX_ENEMY_CARS 2

enum Weather {DAY, NIGHT, SNOW, FOG, SUNSET, SUNRISE};
enum Runway_direction {straight, middle_left, left, middle_right, right};
enum User_input_direction {USER_INPUT_LEFT, USER_INPUT_RIGHT, USER_INPUT_STRAIGHT};
enum Game_state {GAME_RUNNING, GAME_OVER, GAME_PAUSED};

typedef struct image_matrix{
	uint32_t height;
	uint32_t width;
	uint32_t** values;
} Image_matrix;

typedef struct weather_manager {
	enum Weather weather;
	int weather_controller;
	int weather_duration;
	int weather_timer;
} WeatherManager;

typedef struct runway_manager {
	enum Runway_direction runway_direction;
	int runway_direction_duration;
	int runway_direction_timer;
	uint32_t runway_width;
} RunwayManager;

typedef struct game_state {
	Car *player_car;
	Car *enemy_car[MAX_ENEMY_CARS];
	uint32_t max_enemy_cars;
	uint32_t enemy_cars_quantity;
	Mountain *mountain;
	Console *console;
	RunwayManager runway_manager;
	WeatherManager weather_manager;
	uint32_t day_time;
	uint32_t day_duration;
	uint32_t day;
	uint32_t day_max;
	enum Game_state state;
} GameState;

typedef struct scenario {
	uint32_t ground;
	uint32_t sky;
	uint32_t horizon;
	uint32_t mountains;
	uint32_t runway;
	uint32_t mountain_position;
	enum Weather weather;
} Scenario;

Image_matrix* new_matrix_image(uint32_t height, uint32_t width);
void delete_matrix_image(Image_matrix** image);
void invert_image_axis_x(Image_matrix* image_display);
void invert_image_axis_y(Image_matrix* image_display);

void swap(uint32_t* x1, uint32_t* x2);
#endif

#ifndef CAR_H
#define CAR_H

#include "Image.h"
#include <stdbool.h>

#define MAX_CAR_SPEED 4

enum Car_direction {LEFT, RIGHT, STRAIGHT};
enum Collision_directions {COLLISION_LEFT, COLLISION_RIGHT, COLLISION_FRONT};

typedef struct car {
	uint32_t runway_x_position;
	uint32_t runway_y_position;
	uint32_t runway_distance;
	uint32_t collision_correction_speed;
	uint32_t collision_correction_position;
	enum Collision_directions collision_correction_direction;
	bool collision_detected;
	uint32_t speed;
	uint32_t color;
	bool accelerating;
	bool breaking;
	enum Car_direction direction;
	Image* image;
	uint32_t max_speed;
	uint32_t race_position;
	bool visible;
} Car;

extern const int CAR_SMALL_ARRAY[];
extern const int CAR_MEDIUM_ARRAY[];
extern const int CAR_BIG_ARRAY[];

Car *new_car(uint32_t runway_x_position, uint32_t runway_y_position, uint32_t runway_distance, uint32_t speed, uint32_t color, uint32_t size);
void delete_car(Car* car);

#endif
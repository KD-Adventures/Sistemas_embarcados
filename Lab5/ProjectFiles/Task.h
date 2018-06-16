#ifndef TASK_H
#define TASK_H

#include "cmsis_os.h"
#include <string.h>
#include <math.h>

enum TAKS_STATUS {
	READY = 0,
	WAITING,
	RUNNING
};

typedef struct Task {
	osThreadId task_id;
	char name[50];
	
	int status;
	int static_priority;
	int dinamic_priority;
	
	double time_to_sleep; // miliseconds
	double sleeping_time; // miliseconds
	
	int estimated_execution_time; // ticks
	int relaxing_remaining_time; // ticks
	int deadline; // ticks
	
} Task;

Task createTask(char name[], osThreadId thread_id, int static_priority, int requency, int execution_time, int deadline_percentage);


#endif
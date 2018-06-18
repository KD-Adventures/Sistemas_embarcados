#ifndef TASK_H
#define TASK_H

#include "cmsis_os.h"
#include <string.h>
#include <math.h>
#include "Utils.h"

enum TASK_STATUS {
	READY = 0,
	WAITING,
	RUNNING
};

enum TASK_FAULTS {
	MASTER_FAULT = 0,
	SECONDARY_FAULT,
	NO_FAULT
};

enum TASK_PRIORITY {
	REALTIME = 100,
	HIGH = 30,
	NORMAL = 0,
	LOW = 10
};

typedef struct Task {
	osThreadId task_id;
	char name[50];
	
	int status;
	int static_priority;
	int dinamic_priority;
	
	double time_to_sleep; // miliseconds
	double time_to_wakeup; // miliseconds
	
	int executed_time;
	int estimated_execution_time; // ticks
	int relaxing_remaining_time; // ticks
	int deadline; // ticks
	
	int timer;
	
} Task;

Task createTask(char name[], osThreadId thread_id, int static_priority, int requency, int execution_time, int deadline_percentage, enum TASK_STATUS status);

void resetTask(Task* task, enum TASK_STATUS  status);

void sort(Task* tasks[], int size);

#endif
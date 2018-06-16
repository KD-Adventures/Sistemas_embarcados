#include "Task.h"

Task createTask(char name[], osThreadId thread_id, int static_priority, int frequency, int execution_time, int deadline_percentage) {

	Task new_task;
	
	new_task.task_id = thread_id;
	strcpy(new_task.name, name);
	
	new_task.status = READY;
	new_task.static_priority = static_priority;
	new_task.dinamic_priority = static_priority;
	
	new_task.time_to_sleep = 1000/frequency;
	new_task.sleeping_time = new_task.time_to_sleep;
	
	new_task.estimated_execution_time = execution_time;	
	new_task.deadline = floor(new_task.estimated_execution_time*deadline_percentage/100); // ticks
	new_task.relaxing_remaining_time = new_task.deadline - new_task.estimated_execution_time;
	
	return new_task;
}
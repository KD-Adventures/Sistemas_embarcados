#include "Task.h"

Task createTask(char name[], osThreadId thread_id, int static_priority, int frequency, int execution_time, int deadline_percentage, enum TASK_STATUS status) {

	Task new_task;
	
	new_task.task_id = thread_id;
	strcpy(new_task.name, name);
	
	new_task.status = status;
	new_task.static_priority = static_priority;
	new_task.dinamic_priority = static_priority;
	
	new_task.fault = NO_FAULT;
	
	new_task.queue_position = 0;
	
	new_task.time_to_sleep = (int) floor(1000/frequency);
	new_task.time_to_wakeup = new_task.time_to_sleep;
	
	new_task.executed_time = 0;
	new_task.estimated_execution_time = execution_time;	
	new_task.deadline = (int) floor(new_task.estimated_execution_time + new_task.estimated_execution_time*deadline_percentage/100); // ticks
	new_task.relaxing_remaining_time = new_task.deadline - new_task.estimated_execution_time;
	new_task.time_since_start = 0;
	
	new_task.timer = floor(getSystemTime());
	
	return new_task;
}

void resetTask(Task* task, enum TASK_STATUS  status) {
	task->dinamic_priority = task->static_priority;
	task->relaxing_remaining_time = task->deadline - task->estimated_execution_time;
	task->time_since_start = 0;
	task->time_to_wakeup = task->time_to_sleep;
	task->status = status;
	task->timer = (int) floor(getSystemTime());
	task->executed_time = 0;
}

void sort(Task* tasks[], int size) {
	int i, j;
	Task* key;

	for (i = 1; i < size; i++) {
		key = tasks[i];
		j = i-1;

		while (j >= 0 && tasks[j]->dinamic_priority > key->dinamic_priority) {
			tasks[j+1] = tasks[j];
			j = j-1;
		}
		tasks[j+1] = key;
	}
}
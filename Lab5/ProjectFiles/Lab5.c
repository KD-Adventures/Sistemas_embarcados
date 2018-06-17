#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include "uart_functions.h"
#include "cfaf128x128x16.h"
#include <string.h>
#include "Communication.h"
#include <math.h>
#include "Utils.h"
#include "queue.h"
#include "Task.h"
#include "gantt.h"

#define M_PI 3.14159265358979323846
//#define GANTT
#define SIMULATION
#define OS_ROBIN 0

#define SIGNAL_EXECUTE_THREAD 0x0010

//Estimated execution times in SysTicks
#define TASK_A_EXECUTION_TIME 41906
#define TASK_B_EXECUTION_TIME 147138
#define TASK_C_EXECUTION_TIME 26130
#define TASK_D_EXECUTION_TIME 5146
#define TASK_E_EXECUTION_TIME 42902
#define TASK_F_EXECUTION_TIME 2791439

tContext sContext;

// THREADS
void Thread_A (void const *args);
void Thread_B (void const *args);
void Thread_C (void const *args);
void Thread_D (void const *args);
void Thread_E (void const *args);
void Thread_F (void const *args);
void dispatcher();
void dispatcher_run();

osThreadDef (Thread_A, osPriorityNormal, 1, 0);
osThreadDef (Thread_B, osPriorityNormal, 1, 0);
osThreadDef (Thread_C, osPriorityNormal, 1, 0);
osThreadDef (Thread_D, osPriorityNormal, 1, 0);
osThreadDef (Thread_E, osPriorityNormal, 1, 0);
osThreadDef (Thread_F, osPriorityNormal, 1, 0);

Task task_A;
Task task_B;
Task task_C;
Task task_D;
Task task_E;
Task task_F;
Task* current_task;

Task Dispatcher;

osTimerDef(timer, dispatcher_run);
osTimerId timer_id;

osMutexDef (mutex_running_thread);
osMutexId  (mutex_running_thread_id);

void task_yield(Task* running_thread, bool isPreempt);

void dispatcher_run(){
	task_yield(current_task, true);
}

void task_yield(Task* running_thread, bool isPreempt) {
	#ifdef GANTT
	gantt_thread_exit(running_thread->name, osKernelSysTick()/120000);
	#endif
	
	if (running_thread->task_id != Dispatcher.task_id && running_thread->status == RUNNING) {
		if (isPreempt)
			running_thread->status = READY;
		else
			running_thread->status = WAITING;
	}
	
	
	osSignalSet (Dispatcher.task_id, 0x0001);	
}

double factorial (int n) {
	if (n > 1)
		return n*(factorial(n-1));
	else
		return 1.0;
}

void Thread_A (void const *args) {
	int x;
	double soma;
		
	while (true) {
		for(soma = 0, x = 0; x <= 256; x++) {
			while(task_A.status != RUNNING)
				osThreadYield();
			soma += x + (x + 2);
		}
		
		x = 0;
		task_yield(&task_A, false);
	}
}

void Thread_B (void const *args) {
	int n;
	double soma;
	
	while (true) {		
		for(soma = 0, n = 1; n <= 16; n++) {
			while(task_B.status != RUNNING)
				osThreadYield();
			soma += pow(2.0, n) / factorial(n);
		}

		n = 0;
		task_yield(&task_B, false);
	}
}

void Thread_C (void const *args) {
	int n;
	double soma;
	
	while (true) {
		for(soma = 0, n = 1; n <= 72; n++) {
			while(task_C.status != RUNNING)
				osThreadYield();
			soma += (double)(n+1)/(double)n;
		}
		
		n = 0;
		task_yield(&task_C, false);
	}
}

void Thread_D (void const *args) {
	double soma;
	
	while(true) {
		while(task_D.status != RUNNING)
			osThreadYield();
		soma = 1 + (5.0/factorial(3)) + (5.0/factorial(5)) + (5.0/factorial(7)) + (5.0/factorial(9));
		
		soma = 0;
		task_yield(&task_D, false);
	}
}

void Thread_E (void const *args) {
	int x;
	double soma;

	while(true) {
		for(soma = 0, x = 1; x <= 100; x++) {
			while(task_E.status != RUNNING)
				osThreadYield();
			soma += x*M_PI*M_PI;
		}
		
		x = 0;
		task_yield(&task_E, false);
	}
}

void Thread_F (void const *args) {
	int y;
	double soma;
	
	while(true) {
		for(soma = 0, y = 1; y <= 128; y++) {
			while(task_F.status != RUNNING)
				osThreadYield();
			soma += pow((double) y, 3.0) / pow(2.0, (double) y);
		}
		
		y = 0;
		task_yield(&task_F, false);
	}
}

void load_threads(Task* tasks[], int size) {
	int i;
	tasks[0] = &task_A;
	tasks[1] = &task_B;
	tasks[2] = &task_C;
	tasks[3] = &task_D;
	tasks[4] = &task_E;
	tasks[5] = &task_F;
	
	for(i = 0; i < size; i++) {
		tasks[i]->timer = floor(getSystemTime());
	}
}

void updateTasks(Task* ready_tasks[], int *n_ready_tasks, int ready_max_size, Task* waiting_tasks[], int *n_waiting_tasks, int waiting_max_size) {
	int i;
	int previous_time;
	int diff_time;
	Task* element;
	
	for (i = 0; i < *n_ready_tasks; i++) {
		previous_time = ready_tasks[i]->timer;
		ready_tasks[i]->timer = floor(getSystemTime());
		diff_time = floor(ready_tasks[i]->timer - previous_time);
		ready_tasks[i]->relaxing_remaining_time -= diff_time;
		ready_tasks[i]->dinamic_priority--;
	}
	
	for (i = 0; i < *n_waiting_tasks; i++) {
		previous_time = waiting_tasks[i]->timer;
		waiting_tasks[i]->timer = floor(getSystemTime());
		diff_time = floor(waiting_tasks[i]->timer - previous_time);
		waiting_tasks[i]->time_to_wakeup -= diff_time;
		if (waiting_tasks[i]->time_to_wakeup <= 0) {
			element = get_element((void*)waiting_tasks, n_waiting_tasks, i);
			i--;
			element->status = READY;
			push_element((void*)ready_tasks, ready_max_size, element, n_ready_tasks);
		}
	}
}

void scheduler(Task* tasks[], int size) {
	sort(tasks, size);
}

void dispatcher() {
	int number_of_tasks;
	Task* ready_tasks[6];
	Task* waiting_tasks[6];
	Task* previous_task;
	osEvent event;
	int ready_tasks_index = 0;
	int ready_tasks_size = 0;
	int waiting_tasks_index = 0;
	int waiting_tasks_size = 0;
	number_of_tasks = 6;
	waiting_tasks_size = number_of_tasks;
	
	load_threads(waiting_tasks, number_of_tasks);
	
	while(true) {
		event = osSignalWait (0x0001, 1);
		
		if (current_task->status == RUNNING && current_task->task_id != Dispatcher.task_id) {
			continue;
		}
		else if (event.status == osEventSignal) {
			osTimerStop(timer_id);
			osMutexWait(mutex_running_thread_id, osWaitForever);
			
			// Se a task terminou
			if (current_task->status == WAITING) {
				resetTask(current_task, WAITING);
			}
			
			//Adds previous task to the "ready tasks" queue			
			if (current_task > 0 && current_task->status == READY)
				push_element((void*)ready_tasks, number_of_tasks, (void*)current_task, &ready_tasks_size);
			else if (current_task > 0 && current_task->status == WAITING)
				push_element((void*)waiting_tasks, number_of_tasks, (void*)current_task, &waiting_tasks_size);			
			
			//Selects next task to be executed
			updateTasks(ready_tasks, &ready_tasks_size, number_of_tasks, waiting_tasks, &waiting_tasks_size, number_of_tasks);;
			scheduler(ready_tasks, ready_tasks_size);
			previous_task = current_task;
			current_task = (Task*)get_first_element((void*)&ready_tasks, &ready_tasks_size);
			
			//Sets selected task to running mode
			#ifdef GANTT
			gantt_thread_enter(current_task->name, (int)(osKernelSysTick()/120000));
			#endif
			current_task->status = RUNNING;
			
			osMutexRelease(mutex_running_thread_id);
			osTimerStart(timer_id, 10);
		}
		
		osThreadYield();
	}
}

int main (void) {
	#ifdef GANTT
	initUART();
	gantt_generate_header();
	#endif
	
	#ifndef SIMULATION
	cfaf128x128x16Init();
	#endif
	
    osKernelInitialize();
	// ajustar valores para dispatcher
	osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
	Dispatcher = createTask("Dispatcher ", osThreadGetId(), 0, 0, 0, 0, RUNNING);
	Dispatcher.status = RUNNING;
	current_task = &Dispatcher;
	
	// os dados de executiong time are not correct
	task_A = createTask("Task A", osThreadCreate(osThread(Thread_A), NULL), 10, 8, TASK_A_EXECUTION_TIME, 70, WAITING);
	task_B = createTask("Task B", osThreadCreate(osThread(Thread_B), NULL), 0, 2, TASK_B_EXECUTION_TIME, 50, WAITING);
	task_C = createTask("Task C", osThreadCreate(osThread(Thread_C), NULL), -30, 5, TASK_C_EXECUTION_TIME, 30, WAITING);
	task_D = createTask("Task D", osThreadCreate(osThread(Thread_D), NULL), 0, 1, TASK_D_EXECUTION_TIME, 50, WAITING);
	task_E = createTask("Task E", osThreadCreate(osThread(Thread_E), NULL), -30, 6, TASK_E_EXECUTION_TIME, 30, WAITING);
	task_F = createTask("Task F", osThreadCreate(osThread(Thread_F), NULL), -100, 10, TASK_F_EXECUTION_TIME, 10, WAITING);
    osKernelStart();
	
	timer_id = osTimerCreate(osTimer(timer), osTimerPeriodic, NULL);
	mutex_running_thread_id = osMutexCreate(osMutex(mutex_running_thread));
    
	osTimerStart(timer_id, 10);
	dispatcher();
}
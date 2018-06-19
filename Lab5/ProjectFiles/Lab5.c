#include "cmsis_os.h"
#include <stdbool.h>
#include <stdlib.h>
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
//#define SIMULATION
#define DISPLAY_TASK
#define OS_ROBIN 0

#ifdef DISPLAY_TASK
#define NUMBER_OF_TASKS 7
#else
#define NUMBER_OF_TASKS 6
#endif

#define SIGNAL_EXECUTE_THREAD 0x0010

//Estimated execution times in microseconds
const int task_a_execution_time = 349;
const int task_b_execution_time = 1226;
const int task_c_execution_time = 218;
const int task_d_execution_time = 43;
const int task_e_execution_time = 358;
const int task_f_execution_time = 23261;
const int display_task_execution_time = 100000;

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

bool secondary_fault = false, master_fault = false;

#ifdef DISPLAY_TASK
void Thread_Display (void const *args);
osThreadDef (Thread_Display, osPriorityNormal, 1, 0);
Task task_Display;
#endif

osTimerDef(timer, dispatcher_run);
osTimerId timer_id;

osMutexDef (mutex_running_thread);
osMutexId  (mutex_running_thread_id);

void task_yield(Task* running_thread, bool isPreempt);

void dispatcher_run(){
	task_yield(current_task, true);
}

void task_yield(Task* running_thread, bool isPreempt) {
	if (!isPreempt)
		running_thread->status = WAITING;
	else if (running_thread->task_id != Dispatcher.task_id && running_thread->status == RUNNING) {
		running_thread->status = READY;
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

#ifdef DISPLAY_TASK
void draw_table_structure() {
	GrStringDraw(&sContext, "A", -1, (sContext.psFont->ui8MaxWidth)*4, (sContext.psFont->ui8Height+8)*0, true);
	GrStringDraw(&sContext, "B", -1, (sContext.psFont->ui8MaxWidth)*7, (sContext.psFont->ui8Height+8)*0, true);
	GrStringDraw(&sContext, "C", -1, (sContext.psFont->ui8MaxWidth)*10, (sContext.psFont->ui8Height+8)*0, true);
	GrStringDraw(&sContext, "D", -1, (sContext.psFont->ui8MaxWidth)*13, (sContext.psFont->ui8Height+8)*0, true);
	GrStringDraw(&sContext, "E", -1, (sContext.psFont->ui8MaxWidth)*16, (sContext.psFont->ui8Height+8)*0, true);
	GrStringDraw(&sContext, "F", -1, (sContext.psFont->ui8MaxWidth)*19, (sContext.psFont->ui8Height+8)*0, true);
	
	GrStringDraw(&sContext, "P", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*1, true);
	GrStringDraw(&sContext, "R", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*2, true);
	GrStringDraw(&sContext, "S", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*3, true);
	GrStringDraw(&sContext, "%", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*4, true);
	GrStringDraw(&sContext, "D", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*5, true);
	GrStringDraw(&sContext, "Q", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*6, true);
	GrStringDraw(&sContext, "F", -1, (sContext.psFont->ui8MaxWidth)*1, (sContext.psFont->ui8Height+8)*7, true);
}

void copy_task_struct (Task* copy_from, Task* copy_to) {
	copy_to->task_id = copy_from->task_id;
	copy_to->status = copy_from->status;
	copy_to->static_priority = copy_from->static_priority;
	copy_to->dinamic_priority = copy_from->dinamic_priority;
	copy_to->queue_position = copy_from->queue_position;
	copy_to->executed_time = copy_from->executed_time;
	copy_to->estimated_execution_time = copy_from->estimated_execution_time;	
	copy_to->relaxing_remaining_time = copy_from->relaxing_remaining_time;
	copy_to->time_since_start = copy_from->time_since_start;
}

void fill_task_info (Task* task_info) {
	copy_task_struct(&task_A, &task_info[0]);
	copy_task_struct(&task_B, &task_info[1]);
	copy_task_struct(&task_C, &task_info[2]);
	copy_task_struct(&task_D, &task_info[3]);
	copy_task_struct(&task_E, &task_info[4]);
	copy_task_struct(&task_F, &task_info[5]);
}

void show_task_info (Task task_info, int x_pos) {
	char buffer[10];

	/* PRIORITY */
	intToString(task_info.dinamic_priority, buffer, 10, 10, 2);
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*1, true);

	
	/* RELAXING TIME */
	intToString(task_info.relaxing_remaining_time, buffer, 10, 10, 2);
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*2, true);
	
	
	/* STATUS */
	switch(task_info.status) {
		case (READY):
			strcpy(buffer,"R");
			break;
		case (WAITING):
			strcpy(buffer,"W");
			break;
		default:
			strcpy(buffer," ");
			break;
	}
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*(x_pos+2), (sContext.psFont->ui8Height+8)*3, true);
	
	
	/* PERCENT DONE */
	intToString((task_info.executed_time*100/task_info.estimated_execution_time), buffer, 10, 10, 2);
	strcat(buffer, " ");
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*4, true);

	
	/* DELAY */
	if (task_info.time_since_start > task_info.estimated_execution_time)
		intToString((task_info.time_since_start - task_info.estimated_execution_time), buffer, 10, 10, 2);
	else
		strcpy(buffer, "00");
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*5, true);
	
	
	/* QUEUE POSITION */
	intToString(task_info.queue_position, buffer, 10, 10, 2);
	GrStringDraw(&sContext, buffer, -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*6, true);
	
	
	/* FAULTS */
	if (master_fault) {
		master_fault = false;
		secondary_fault = false;
		GrStringDraw(&sContext, "MASTER FLT", -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*7, true);
	}
	else if (secondary_fault) {
		secondary_fault = false;
		GrStringDraw(&sContext, "SECONDARY FLT", -1, (sContext.psFont->ui8MaxWidth)*x_pos, (sContext.psFont->ui8Height+8)*7, true);
	}
}

void Thread_Display (void const *args) {
	int task_index, x_pos, y_pos;
	Task task_info[6];//, task_B_info, task_C_info, task_D_info, task_E_info, task_F_info;
	
	while(task_Display.status != RUNNING)
		osThreadYield();
	
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, &g_sFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	draw_table_structure();
	
	while(true) {
		while(task_Display.status != RUNNING)
			osThreadYield();
		
		fill_task_info(task_info);
		
		//Magic numbers
		for (x_pos = 2, task_index = 0; task_index < 6; task_index++) {
			while(task_Display.status != RUNNING)
				osThreadYield();
			
			show_task_info(task_info[task_index], x_pos);
			x_pos += 3;
		}
		
		//Being the longest task, it is necessary to ensure that task_Display is at RUNNING state.
		while(task_Display.status != RUNNING)
			osThreadYield();
		task_yield(&task_Display, false);
	}
}
#endif

void load_threads(Task* tasks[], int size) {
	int i;
	tasks[0] = &task_A;
	tasks[1] = &task_B;
	tasks[2] = &task_C;
	tasks[3] = &task_D;
	tasks[4] = &task_E;
	tasks[5] = &task_F;
	#ifdef DISPLAY_TASK
	tasks[6] = &task_Display;
	#endif
	
	for(i = 0; i < size; i++) {
		tasks[i]->timer = floor(getSystemTime());
	}
}

enum TASK_FAULTS analyze_task(Task* task) {
	if (task->executed_time > task->deadline) {
		if (task->static_priority == REALTIME) {
			master_fault = true;
			return MASTER_FAULT;
		}
		task->static_priority -= 10;
		secondary_fault = true;
		return SECONDARY_FAULT;
	}
	else if(task->executed_time < task->deadline/2) {
		task->static_priority += 10;
		secondary_fault = true;
		return SECONDARY_FAULT;
	}
	
	return NO_FAULT;
}

void updateTasks(Task* ready_tasks[], int *n_ready_tasks, int ready_max_size, Task* waiting_tasks[], int *n_waiting_tasks, int waiting_max_size) {
	int i;
	int previous_time;
	int diff_time;
	int increase_priority;
	Task* element;
	
	for (i = 0; i < *n_ready_tasks; i++) {
		previous_time = ready_tasks[i]->timer;
		ready_tasks[i]->timer = (int) floor(getSystemTime());
		diff_time = floor(ready_tasks[i]->timer - previous_time);
		ready_tasks[i]->relaxing_remaining_time -= diff_time + ready_tasks[i]->executed_time;
		
		increase_priority = abs((int) ceil(1000000/ready_tasks[i]->relaxing_remaining_time));
		if (increase_priority > 100) {
			increase_priority = 100;
		}
		
		ready_tasks[i]->dinamic_priority = ready_tasks[i]->static_priority - increase_priority;
		
		ready_tasks[i]->time_since_start += diff_time;
		if(ready_tasks[i] == current_task)
			ready_tasks[i]->executed_time += diff_time;
	}
	
	for (i = 0; i < *n_waiting_tasks; i++) {
		previous_time = waiting_tasks[i]->timer;
		waiting_tasks[i]->timer = floor(getSystemTime());
		diff_time = floor(waiting_tasks[i]->timer - previous_time);
		waiting_tasks[i]->time_to_wakeup -= diff_time;
		waiting_tasks[i]->time_since_start += diff_time;
		waiting_tasks[i]->queue_position = 0;
		if (waiting_tasks[i]->time_to_wakeup <= 0) {
			element = get_element((void*)waiting_tasks, n_waiting_tasks, i);
			i--;
			element->status = READY;
			push_element((void*)ready_tasks, ready_max_size, element, n_ready_tasks);
		}
	}
}

void scheduler(Task* tasks[], int size) {
	int i;
	sort(tasks, size);
	
	for (i = 0; i < size; i++) {
		tasks[i]->queue_position = i+1;
	}
}

void dispatcher() {
	int number_of_tasks;
	Task* ready_tasks[NUMBER_OF_TASKS];
	Task* waiting_tasks[NUMBER_OF_TASKS];
	Task* previous_task;
	osEvent event;
	enum TASK_FAULTS fault;
	int diff_time;
	int ready_tasks_index = 0;
	int ready_tasks_size = 0;
	int waiting_tasks_index = 0;
	int waiting_tasks_size = 0;
	number_of_tasks = NUMBER_OF_TASKS;
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
			
			if (current_task != &Dispatcher) {
				diff_time = floor(getSystemTime() - current_task->timer);
				current_task->executed_time += diff_time;
				
				//Task is finished
				if (current_task->status == WAITING) {
					fault = analyze_task(current_task);
					resetTask(current_task, WAITING);
				}
				
				//Adds previous task to the "ready tasks" queue			
				if (current_task > 0 && current_task->status == READY)
					push_element((void*)ready_tasks, number_of_tasks, (void*)current_task, &ready_tasks_size);
				else if (current_task > 0 && current_task->status == WAITING)
					push_element((void*)waiting_tasks, number_of_tasks, (void*)current_task, &waiting_tasks_size);			
			}
				
			//Selects next task to be executed
			updateTasks(ready_tasks, &ready_tasks_size, number_of_tasks, waiting_tasks, &waiting_tasks_size, number_of_tasks);;
			scheduler(ready_tasks, ready_tasks_size);
			previous_task = current_task;
			current_task = (Task*)get_first_element((void*)&ready_tasks, &ready_tasks_size);
			
			#ifdef GANTT
			if (previous_task == &Dispatcher) {
				gantt_thread_enter(current_task->name, (int) getSystemTime());
			}
			else if (current_task != previous_task) {
				gantt_thread_exit(previous_task->name, (int) getSystemTime());
				gantt_thread_enter(current_task->name, (int) getSystemTime());
			}
			#endif
			
			//Sets selected task to running mode
			current_task->status = RUNNING;
			
			osMutexRelease(mutex_running_thread_id);
			osTimerStart(timer_id, 10);
		}
		
		osThreadYield();
	}
}

int main (void) {	
	#ifndef SIMULATION
	cfaf128x128x16Init();
	#endif
	
	#ifdef GANTT
	initUART();
	gantt_generate_header();
	#endif
	
    osKernelInitialize();
	osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
	Dispatcher = createTask("Dispatcher ", osThreadGetId(), 0, 0, 0, 0, RUNNING);
	Dispatcher.status = RUNNING;
	current_task = &Dispatcher;
	task_A = createTask("Task A", osThreadCreate(osThread(Thread_A), NULL), 10, 8, task_a_execution_time, 70, WAITING);
	task_B = createTask("Task B", osThreadCreate(osThread(Thread_B), NULL), 0, 2, task_b_execution_time, 50, WAITING);
	task_C = createTask("Task C", osThreadCreate(osThread(Thread_C), NULL), -30, 5, task_c_execution_time, 30, WAITING);
	task_D = createTask("Task D", osThreadCreate(osThread(Thread_D), NULL), 0, 1, task_d_execution_time, 50, WAITING);
	task_E = createTask("Task E", osThreadCreate(osThread(Thread_E), NULL), -30, 6, task_e_execution_time, 30, WAITING);
	task_F = createTask("Task F", osThreadCreate(osThread(Thread_F), NULL), -100, 10, task_f_execution_time, 10, WAITING);
	#ifdef DISPLAY_TASK
	task_Display = createTask("Display Task", osThreadCreate(osThread(Thread_Display), NULL), -50, 1, display_task_execution_time, 10, WAITING);
	#endif
    osKernelStart();
	
	timer_id = osTimerCreate(osTimer(timer), osTimerPeriodic, NULL);
	mutex_running_thread_id = osMutexCreate(osMutex(mutex_running_thread));
    
	osTimerStart(timer_id, 1);
	dispatcher();
}
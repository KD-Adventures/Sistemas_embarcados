/*============================================================================
 *                    Exemplos de utilização do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. André Schneider de Oliveira
 *            Universidade Tecnológica Federal do Paraná (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 *      Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 *      Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 *      Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 *      Mariana Carrião        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/
#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include "uart_functions.h"
#include "cfaf128x128x16.h"
#include <string.h>
#include "Comunication.h"
#include <math.h>
#include "Utils.h"
#include "queue.h"
#include "Task.h"

#define M_PI 3.14159265358979323846
//#define GANTT
#define SIMULATION
#define OS_ROBIN 0

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

osThreadId Dispatcher_id;

osTimerDef(timer, dispatcher_run);
osTimerId timer_id;

osMutexDef (mutex_running_thread);
osMutexId  (mutex_running_thread_id);

void dispatcher_run(){
	osSignalSet (Dispatcher_id, 0x0001);
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
			soma += x + (x + 2);
		}
		x = 0;
	}
}

void Thread_B (void const *args) {
	int n;
	double soma;
	
	while (true) {
		
		for(soma = 0, n = 1; n <= 16; n++) {
			soma += pow(2.0, n) / factorial(n);
		}

		n = 0 ;
	}
}

void Thread_C (void const *args) {
	int n;
	double soma;
	
	while (true) {
		
		for(soma = 0, n = 1; n <= 72; n++) {
			soma += (double)(n+1)/(double)n;
		}
		n = 0;
	}
}

void Thread_D (void const *args) {
	double soma;
	soma = 1 + (5.0/factorial(3)) + (5.0/factorial(5)) + (5.0/factorial(7)) + (5.0/factorial(9));
	
	soma = 0;
}

void Thread_E (void const *args) {
	int x;
	double soma;

	for(soma = 0, x = 1; x <= 100; x++) {
		soma += x*M_PI*M_PI;
	}
	x = 0;
}

void Thread_F (void const *args) {
	int y;
	double soma;
	
	for(soma = 0, y = 1; y <= 128; y++) {
		soma += pow((double) y, 3.0) / pow(2.0, (double) y);
	}
	y = 0;
}

void task_yield(Task* running_thread) {
}

void load_threads(Task* tasks[]) {
	tasks[0] = &task_A;
	tasks[1] = &task_B;
	tasks[2] = &task_C;
	//tasks[3] = task_D;
	//tasks[4] = task_E;
	//tasks[5] = task_F;
}

void scheduler(Task* tasks[], int size) {
	// reorder
}

void dispatcher() {
	int number_of_tasks;
	Task* ready_tasks[3];
	Task* waiting_tasks[3];
	Task* current_task;
	Task* previous_task;
	osEvent event;
	int ready_tasks_index = 0;
	int ready_tasks_size;
	int waiting_tasks_index = 0;
	number_of_tasks = 3;
	ready_tasks_size = number_of_tasks;
	
	
	load_threads(ready_tasks);
	
	while(true) {
		event = osSignalWait (0x0001, osWaitForever);
		if (event.status == osEventSignal) {
			osMutexWait(mutex_running_thread_id, osWaitForever);
			
			put_element((void*)ready_tasks, number_of_tasks, (void*)&current_task, &ready_tasks_size);
			
			scheduler(ready_tasks, number_of_tasks);
			
			current_task = (Task*)get_first_element((void*)&ready_tasks, &ready_tasks_size);
			
			osMutexRelease(mutex_running_thread_id);
			
		}
	}
}

int main (void) {
	osThreadId Thread_A_id;
	osThreadId Thread_B_id;
	osThreadId Thread_C_id;
	osThreadId Thread_D_id;
	osThreadId Thread_E_id;
	osThreadId Thread_F_id;
	
    #ifdef GANTT
	initUART();
	#endif
	
#ifndef SIMULATION
	cfaf128x128x16Init();
#endif

    osKernelInitialize();
	Dispatcher_id = osThreadGetId();
	Thread_A_id = osThreadCreate(osThread(Thread_A), NULL);
	Thread_B_id = osThreadCreate(osThread(Thread_B), NULL); 
	Thread_C_id = osThreadCreate(osThread(Thread_C), NULL);
	Thread_D_id = osThreadCreate(osThread(Thread_D), NULL);
	Thread_E_id = osThreadCreate(osThread(Thread_E), NULL);
	Thread_F_id = osThreadCreate(osThread(Thread_F), NULL);
    osKernelStart();
	
	// os dados de executiong time are not correct
	task_A = createTask("Task A", Thread_A_id, 10, 8, 1000, 70);
	task_B = createTask("Task B", Thread_B_id, 0, 2, 1000, 50);
	task_C = createTask("Task C", Thread_C_id, -30, 5, 1000, 30);
	task_D = createTask("Task D", Thread_D_id, 0, 1, 1000, 50);
	task_E = createTask("Task E", Thread_E_id, -30, 6, 1000, 30);
	task_F = createTask("Task F", Thread_F_id, -100, 10, 1000, 10);
	
	timer_id = osTimerCreate(osTimer(timer), osTimerPeriodic, NULL);
	mutex_running_thread_id = osMutexCreate(osMutex(mutex_running_thread));
    
	osTimerStart(timer_id, 500);
	dispatcher();
}
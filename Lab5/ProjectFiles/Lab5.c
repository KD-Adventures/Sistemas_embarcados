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

#define M_PI 3.14159265358979323846
#define GANTT
#define OS_ROBIN 0

tContext sContext;

// THREADS
void Thread_A (void const *args);
void Thread_B (void const *args);
void Thread_C (void const *args);
void Thread_D (void const *args);
void Thread_E (void const *args);
void Thread_F (void const *args);
void scheduler();

osThreadDef (Thread_A, osPriorityNormal, 1, 0);
osThreadDef (Thread_B, osPriorityNormal, 1, 0);
osThreadDef (Thread_C, osPriorityNormal, 1, 0);
osThreadDef (Thread_D, osPriorityNormal, 1, 0);
osThreadDef (Thread_E, osPriorityNormal, 1, 0);
osThreadDef (Thread_F, osPriorityNormal, 1, 0);

osThreadId Thread_A_id;
osThreadId Thread_B_id;
osThreadId Thread_C_id;
osThreadId Thread_D_id;
osThreadId Thread_E_id;
osThreadId Thread_F_id;
osThreadId Scheduler_id;

osTimerDef(timer, scheduler);
osTimerId timer_id;

double factorial (int n) {
	if (n > 1)
		return n*(factorial(n-1));
	else
		return 1.0;
}

void Thread_A (void const *args) {
	int x;
	double soma;

	for(soma = 0, x = 0; x <= 256; x++) {
		soma += x + (x + 2);
	}
}

void Thread_B (void const *args) {
	int n;
	double soma;

	for(soma = 0, n = 1; n <= 16; n++) {
		soma += pow(2.0, n) / factorial(n);
	}

}

void Thread_C (void const *args) {
	int n;
	double soma;
	
	for(soma = 0, n = 1; n <= 72; n++) {
		soma += (double)(n+1)/(double)n;
	}
}

void Thread_D (void const *args) {
	double soma;
	soma = 1 + (5.0/factorial(3)) + (5.0/factorial(5)) + (5.0/factorial(7)) + (5.0/factorial(9));
}

void Thread_E (void const *args) {
	int x;
	double soma;

	for(soma = 0, x = 1; x <= 100; x++) {
		soma += x*M_PI*M_PI;
	}
}

void Thread_F (void const *args) {
	int y;
	double soma;
	
	for(soma = 0, y = 1; y <= 128; y++) {
		soma += pow((double) y, 3.0) / pow(2.0, (double) y);
	}	
}

void scheduler() {
	
}

int main (void) {
    #ifdef GANTT
	initUART();
	#endif
	
	cfaf128x128x16Init();

    osKernelInitialize();
	Scheduler_id = osThreadGetId();
	Thread_A_id = osThreadCreate(osThread(Thread_A), NULL);
	Thread_B_id = osThreadCreate(osThread(Thread_B), NULL); 
	Thread_C_id = osThreadCreate(osThread(Thread_C), NULL);
	Thread_D_id = osThreadCreate(osThread(Thread_D), NULL);
	Thread_E_id = osThreadCreate(osThread(Thread_E), NULL);
	Thread_F_id = osThreadCreate(osThread(Thread_F), NULL);
    osKernelStart();
    
	while(true)
		scheduler();
}
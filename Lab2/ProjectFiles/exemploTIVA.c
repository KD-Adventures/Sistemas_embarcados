/*============================================================================
 *                    LAB 2 - Sistemas Embarcados - S72                      *
 *  Autores: Davi Boberg
 *           Lucas Kaminski de Freitas
 *---------------------------------------------------------------------------*
 * Autores das bibliotecas:
 *      Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 *      Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 *      Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 *      Mariana Carri�o        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os.h"

#define TAMANHO_FILA 500

void imprime_mensagem(void const *args);
osThreadDef (imprime_mensagem, osPriorityNormal, 1, 0);

//--------------------------------
// Fila de espera de processamento
//--------------------------------
typedef struct fila {
	int elementos[TAMANHO_FILA];
	int primeiro_elemento;
	int proxima_posicao_vazia;
	bool full;
	bool empty;
} fila_espera;


//--------------------------------
// Declara��o de vari�veis globais
//--------------------------------
fila_espera *fila_primos, *fila_chaves;
bool chave_valida = false;
bool decodificado_tail = false;
bool fim_teste_ultima = false;
bool fim_teste_penultima = false;
bool mensagem_decodificada = false;
bool imprimindo_mensagem = false;
FILE* log_file = NULL;
int previous_thread = -1;

//-----------------
// Fun��es de filas
//-----------------
void criar_fila (fila_espera *fila) {
	fila->primeiro_elemento = 0;
	fila->proxima_posicao_vazia = 0;
	fila->full = false;
	fila->empty = true;
}

void enqueue (fila_espera *fila, int elemento) {
	fila->elementos[fila->proxima_posicao_vazia] = elemento;
	fila->proxima_posicao_vazia++;

	if (fila->proxima_posicao_vazia == TAMANHO_FILA)
		fila->proxima_posicao_vazia = 0;

	fila->empty = false;
	if (fila->proxima_posicao_vazia == fila->primeiro_elemento)
		fila->full = true;
}

int dequeue (fila_espera *fila) {
	int elemento = fila->elementos[fila->primeiro_elemento];
	fila->primeiro_elemento++;

	if (fila->primeiro_elemento == TAMANHO_FILA)
		fila->primeiro_elemento = 0;

	fila->full = false;
	if (fila->proxima_posicao_vazia == fila->primeiro_elemento)
		fila->empty = true;

	return elemento;
}


void print_log(int thread_index) {
	/*if(previous_thread < 0) {
		fprintf(log_file, "		thread %d, 1d", thread_index);
		previous_thread = thread_index;
		return;
	}
	fprintf(log_file, "		thread %d, :after %d, 1d", thread_index, previous_thread);
	previous_thread = thread_index;*/
	int i;
	for(i = 0; i < 7000; i++);
	return;
}

void criar_thread_imprime_mensagem () {
	if (fim_teste_penultima && fim_teste_ultima && mensagem_decodificada && !imprimindo_mensagem) {
		imprimindo_mensagem = true;
		osThreadCreate(osThread(imprime_mensagem), NULL);
	}
}

//------------------------------------------------------------------------------------
// Thread respons�vel por encher a fila_chaves com n�meros inteiros em ordem crescente
//------------------------------------------------------------------------------------
void gerar_chaves(void const *args){
	int chave = 2;

	while (!chave_valida) {
		if (fila_chaves->full) {
			osThreadYield();
			continue;
		}
		
		enqueue (fila_chaves, chave);
		chave++;
		print_log(1);
	}

	free (fila_chaves);
}
osThreadDef (gerar_chaves, osPriorityNormal, 1, 0);


//--------------------------------------------------------------------------
// Thread respons�vel por verificar se os n�meros da fila_chaves s�o primos.
// Se sim, esses n�meros s�o salvos na fila_primos.
//--------------------------------------------------------------------------
void verificar_primos (void const *args) {
	int i, atual;
	bool tem_divisor;

	while (!chave_valida) {
		if (fila_primos->full || fila_chaves->empty) {
			osThreadYield();
			continue;
		}
		
		atual = dequeue (fila_chaves);

		for (i = 2, tem_divisor = false; i < (atual / 2)+1; i++) {
			if (atual % i == 0) {
				tem_divisor = true;
				break;
			}
		}

		if (!tem_divisor)
			enqueue(fila_primos, atual);
		print_log(2);
	}
	free (fila_primos);
}
osThreadDef (verificar_primos, osPriorityNormal, 1, 0);

void testar_penultima_word(void const *args){
	if(decodificado_tail) {
		print_log(4);
		fim_teste_penultima = true;
		criar_thread_imprime_mensagem();
	}
}
osThreadDef (testar_penultima_word, osPriorityNormal, 1, 0);

void testar_ultima_word(void const *args){
	if(decodificado_tail) {
		print_log(5);
		fim_teste_ultima = true;
		criar_thread_imprime_mensagem();
	}
}
osThreadDef (testar_ultima_word, osPriorityNormal, 1, 0);

void decodificar_mensagem(void const *args){
	if(decodificado_tail) {
		print_log(6);
		mensagem_decodificada = true;
		criar_thread_imprime_mensagem();
	}
}
osThreadDef (decodificar_mensagem, osPriorityNormal, 1, 0);

void decodificar_tail(void const *args){
	while (!chave_valida) {
		if (fila_primos->empty) {
			osThreadYield();
			continue;
		}
		dequeue(fila_primos);
		print_log(3);
		decodificado_tail = true;
		osThreadCreate(osThread(testar_penultima_word), NULL);
		osThreadCreate(osThread(testar_ultima_word), NULL);
		osThreadCreate(osThread(decodificar_mensagem), NULL);
		osThreadTerminate(osThreadGetId());
	}
}
osThreadDef (decodificar_tail, osPriorityNormal, 1, 0);

void imprime_mensagem(void const *args){
	while (!chave_valida) {
		if (!fim_teste_ultima || !fim_teste_penultima || !mensagem_decodificada) {
			osThreadYield();
			continue;
		}

		print_log(7);

		imprimindo_mensagem = false;
		decodificado_tail = false;
		fim_teste_ultima = false;
		fim_teste_penultima = false;
		mensagem_decodificada = false;
		osThreadCreate(osThread(decodificar_tail), NULL);
		osThreadTerminate(osThreadGetId());
	}
}
//osThreadDef (imprime_mensagem, osPriorityNormal, 1, 0);

int main(void) {
	fila_chaves = (fila_espera*) malloc (sizeof (fila_espera));
	fila_primos = (fila_espera*) malloc (sizeof (fila_espera));
	
	criar_fila (fila_primos);
	criar_fila (fila_chaves);

	osKernelInitialize();
	osThreadCreate(osThread(gerar_chaves), NULL);
	osThreadCreate(osThread(verificar_primos), NULL);
	osThreadCreate(osThread(decodificar_tail), NULL);	
	osKernelStart();
	
	//fprintf(log_file, "gantt\n\ttitle Decodificacao");
	osDelay(osWaitForever);
}

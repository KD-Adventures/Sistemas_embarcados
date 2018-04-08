/*============================================================================
 *                    LAB 2 - Sistemas Embarcados - S72                      *
 *  Autores: Davi Boberg
 *           Lucas Kaminski de Freitas
 *---------------------------------------------------------------------------*
 * Autores das bibliotecas:
 *      Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 *      Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 *      Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 *      Mariana Carriao        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "TM4C129.h"
#include "cmsis_os.h"
#include <stdint.h>

#define TAMANHO_FILA 500

const uint32_t mensagem[] = {
    0x5a990100, 0x6667feff, 0x76990100, 0x6867feff, 0x7b990100, 
    0x6167feff, 0x27990100, 0x2667feff, 0x27990100, 0x4c67feff,
    0x68990100, 0x6767feff, 0x7b990100, 0x5a67feff, 0x75990100,
    0x5a67feff, 0x27990100, 0x5f67feff, 0x6c990100, 0x5a67feff, 
    0x7b990100, 0x2767feff, 0x27990100, 0x4b67feff, 0x76990100,
    0x5b67feff, 0x27990100, 0x4d67feff, 0x6f990100, 0x6867feff,
    0x74990100, 0x5a67feff, 0x7a990100, 0x8a650200, 0x8ebffeff
};
const int mensagem_len = 35;

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
// Declaracao de variaveis globais
//--------------------------------
fila_espera *fila_primos, *fila_chaves;
bool passou_teste_ultima = false;
bool passou_teste_penultima = false;
bool chave_valida = false;
bool decodificado_tail = false;
bool fim_teste_ultima = false;
bool fim_teste_penultima = false;
bool mensagem_decodificada = false;
bool imprimindo_mensagem = false;
int chave_anterior = 0;
int chave_atual = 0;
uint32_t ultima_word_decodificada, penultima_word_decodificada;

//-----------------
// Funcoes de filas
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
	int i;
	for(i = 0; i < 7000; i++);
	return;
}

uint32_t decode(const uint32_t *msg, int index) {
	if (index%2 == 0)
		return msg[index] - chave_atual;
	else
		return msg[index] + chave_atual;
}

//------------------------------------------------------------------------------------
// Thread responsavel por encher a fila_chaves com numeros inteiros em ordem crescente
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
// Thread responsavel por verificar se os numeros da fila_chaves sao primos.
// Se sim, esses numeros sao salvos na fila_primos.
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

		for (i = 2, tem_divisor = false; i <= (int)sqrt((double) atual); i++) {
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

//--------------------------------------------------------------------------
// Thread responsavel por decodificar o final da mensagem, para realizar os
// testes e decidir se e necessário decodificar o resto.
//--------------------------------------------------------------------------
void decodificar_tail(void const *args){
	while (!chave_valida) {
		if (fila_primos->empty || decodificado_tail) {
			osThreadYield();
			continue;
		}
		chave_anterior = chave_atual;
		chave_atual = dequeue(fila_primos);
		ultima_word_decodificada = decode(mensagem, mensagem_len - 1);
		penultima_word_decodificada = decode(mensagem, mensagem_len - 2);
		
		print_log(3);
		decodificado_tail = true;
	}
}
osThreadDef (decodificar_tail, osPriorityNormal, 1, 0);

void testar_penultima_word(void const *args){
	while(!chave_valida) {
		if(!decodificado_tail || fim_teste_penultima) {
			osThreadYield();
			continue;
		}
		if (penultima_word_decodificada == chave_atual / 2)
			passou_teste_penultima = true;
		else
			passou_teste_penultima = false;
		
		print_log(4);
		fim_teste_penultima = true;
	}
}
osThreadDef (testar_penultima_word, osPriorityNormal, 1, 0);

void testar_ultima_word(void const *args){
	while(!chave_valida) {
		if(!decodificado_tail || fim_teste_ultima) {
			osThreadYield();
			continue;
		}
		
		if (ultima_word_decodificada == ((chave_atual * chave_atual) / chave_anterior))
			passou_teste_ultima = true;
		else
			passou_teste_ultima = false;
		
		print_log(5);
		fim_teste_ultima = true;
	}
}
osThreadDef (testar_ultima_word, osPriorityNormal, 1, 0);

void decodificar_mensagem(void const *args){
	int i;
	
	while(!chave_valida) {
		if(!decodificado_tail || mensagem_decodificada) {
			osThreadYield();
			continue;
		}
		
		for (i = 0; i < mensagem_len - 2; i++) {
			// vai ter que criar uma variável global pra armazenar a mensagem decodificada
			// e usar malloc pq o tamanho muda pra cada mensagem.
		}
		
		print_log(6);
		mensagem_decodificada = true;
	}
}
osThreadDef (decodificar_mensagem, osPriorityNormal, 1, 0);

void imprime_mensagem(void const *args){
	while (!chave_valida) {
		if (!fim_teste_ultima || !fim_teste_penultima || !mensagem_decodificada) {
			osThreadYield();
			continue;
		}
		if (passou_teste_ultima && passou_teste_penultima)
			chave_valida = true;
		
		print_log(7);
		decodificado_tail = false;
		fim_teste_ultima = false;
		fim_teste_penultima = false;
		mensagem_decodificada = false;
	}
}
osThreadDef (imprime_mensagem, osPriorityNormal, 1, 0);

int main(void) {
	fila_chaves = (fila_espera*) malloc (sizeof (fila_espera));
	fila_primos = (fila_espera*) malloc (sizeof (fila_espera));
	
	criar_fila (fila_primos);
	criar_fila (fila_chaves);

	osKernelInitialize();
	osThreadCreate(osThread(gerar_chaves), NULL);
	osThreadCreate(osThread(verificar_primos), NULL);
	osThreadCreate(osThread(decodificar_tail), NULL);
	osThreadCreate(osThread(testar_penultima_word), NULL);
	osThreadCreate(osThread(testar_ultima_word), NULL);
	osThreadCreate(osThread(decodificar_mensagem), NULL);
	osThreadCreate(osThread(imprime_mensagem), NULL);
	osKernelStart();
	
	osDelay(osWaitForever);
}

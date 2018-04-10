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
#include <stdio.h>

#define TAMANHO_FILA 500
#define TICK_FACTOR 100000

uint32_t mensagem[] = {
	0x0001995a ,  0xfffe6766 ,  0x00019976 ,  0xfffe6768 ,  0x0001997b ,
	0xfffe6761 ,  0x00019927 ,  0xfffe6726 ,  0x00019927 ,  0xfffe674c ,
	0x00019968 ,  0xfffe6767 ,  0x0001997b ,  0xfffe675a ,  0x00019975 ,
	0xfffe675a ,  0x00019927 ,  0xfffe675f ,  0x0001996c ,  0xfffe675a ,
	0x0001997b ,  0xfffe6727 ,  0x00019927 ,  0xfffe674b ,  0x00019976 ,
	0xfffe675b ,  0x00019927 ,  0xfffe674d ,  0x0001996f ,  0xfffe6768 ,
	0x00019974 ,  0xfffe675a ,  0x0001997a ,  0x0002658a ,  0xfffebf8e
};



//const int mensagem_len = 35;
int mensagem_len = 35;

char mensagem_decodificada[35];
int mensagem_decodificada_len = -1;

//--------------------------------
// Fila de espera de processamento
//--------------------------------
typedef struct fila {
	int elementos[TAMANHO_FILA];
	int primeiro_elemento;
	int proxima_posicao_vazia;
	bool full;
	bool empty;
	bool mutex;
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
bool is_mensagem_decodificada = false;
bool imprimindo_mensagem = false;
int chave_anterior = 0;
int chave_atual = 0;
uint32_t ultima_word_decodificada, penultima_word_decodificada;

uint32_t initTime = 0, final_time = 0;

//-----------------
// Funcoes de filas
//-----------------
void criar_fila (fila_espera *fila) {
	fila->primeiro_elemento = 0;
	fila->proxima_posicao_vazia = 0;
	fila->full = false;
	fila->empty = true;
	fila->mutex = false;
}

void enqueue (fila_espera *fila, int elemento) {
	if(fila->mutex == true)
		osThreadYield();
	
	fila->mutex = true;
	
	fila->elementos[fila->proxima_posicao_vazia] = elemento;
	fila->proxima_posicao_vazia++;

	if (fila->proxima_posicao_vazia == TAMANHO_FILA)
		fila->proxima_posicao_vazia = 0;

	fila->empty = false;
	if (fila->proxima_posicao_vazia == fila->primeiro_elemento)
		fila->full = true;
	
	fila->mutex = false;
}

int dequeue (fila_espera *fila) {
	int elemento = fila->elementos[fila->primeiro_elemento];
	
	if(fila->mutex == true)
		osThreadYield();
	
	fila->mutex = true;
	
	fila->primeiro_elemento++;

	if (fila->primeiro_elemento == TAMANHO_FILA)
		fila->primeiro_elemento = 0;

	fila->full = false;
	if (fila->proxima_posicao_vazia == fila->primeiro_elemento)
		fila->empty = true;
	
	fila->mutex = false;
	return elemento;
}

void print_log(int thread_index) {
	int i;
	for(i = 0; i < 1; i++);
	return;
}

uint32_t decode(const uint32_t *msg, int index) {
	if (index%2 == 0)
		return msg[index] - chave_atual;
	else
		return msg[index] + chave_atual;
}


int fputc(int c, FILE *f) {
  return(ITM_SendChar(c));
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
		ultima_word_decodificada = mensagem[mensagem_len-1] + chave_atual; //decode(mensagem, mensagem_len - 1);
		penultima_word_decodificada = mensagem[mensagem_len-2] - chave_atual;
		
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
		{
			passou_teste_penultima = true;
		}
		else
		{
			passou_teste_penultima = false;
		}  
		
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
		
		if (ultima_word_decodificada == (chave_atual * chave_atual / chave_anterior))
		{
			passou_teste_ultima = true;
		}
		else
		{
			passou_teste_ultima = false;
		}
		
		print_log(5);
		fim_teste_ultima = true;
	}
}
osThreadDef (testar_ultima_word, osPriorityNormal, 1, 0);

void decodificar_mensagem(void const *args){
	int i;
	
	while(!chave_valida) {
		if(!decodificado_tail || is_mensagem_decodificada) {
			osThreadYield();
			continue;
		}
		
		for (i = 0; i < mensagem_len - 2; i++) {
			mensagem_decodificada[i] = decode(mensagem, i);
            // vai ter que criar uma variável global pra armazenar a mensagem decodificada
			// e usar malloc pq o tamanho muda pra cada mensagem.
		}
		mensagem_decodificada[mensagem_len - 1] = '\0';
		mensagem_decodificada_len = mensagem_len - 2;
		is_mensagem_decodificada = true;
	}
}
osThreadDef (decodificar_mensagem, osPriorityNormal, 1, 0);

void imprime_mensagem(void const *args){
	while (!chave_valida) {
		if (!fim_teste_ultima || !fim_teste_penultima || !is_mensagem_decodificada) {
			osThreadYield();
			continue;
		}
		if (passou_teste_ultima && passou_teste_penultima)
		{
			chave_valida = true;
		}
		
		printf("\nMensagem decodificada: %s", mensagem_decodificada);
		printf("\nChave: %d", chave_atual);
		decodificado_tail = false;
		fim_teste_ultima = false;
		fim_teste_penultima = false;
		is_mensagem_decodificada = false;
	}
	final_time = osKernelSysTick()/100000;
	printf("\nTempo total: %d", final_time);
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
	initTime = osKernelSysTick()/100000;
	
	osDelay(osWaitForever);
}

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
//#include <time.h>

#define TAMANHO_FILA 100
#define TICK_FACTOR 100000

uint32_t mensagem[] = {
	0xad464fcd, 0x52b9b0e7, 0xad465000, 0x52b9b0d8, 0xad464ff0, 0x52b9b095, 0xad464fde, 0x52b9b0e5, 0xad464ffd, 0x52b9b0de, 0xad464ff9, 0x52b9b0dc, 0xad464ffe, 0x52b9b0e9, 0xad464ff0, 0x52b9b0da, 0xad464ff9, 0x52b9b095, 0xad464fb8, 0x52b9b095, 0xad464fcd, 0x52b9b0e4, 0xad464ffd, 0x52b9b0e3, 0xad464fab, 0x52b9b0e9, 0xad464ffa, 0x52b9b095, 0xad464fdd, 0x52b9b0ea, 0xad464ff9, 0x52b9b095, 0xad464fab, 0x03e97750, 0x52b9b075
};



//const int mensagem_len = 35;
int mensagem_len = 35;

char mensagem_decodificada[35];
int mensagem_decodificada_len = -1;

//--------------------------------
// Fila de espera de processamento
//--------------------------------
typedef struct fila {
	uint32_t elementos[TAMANHO_FILA];
	uint32_t primeiro_elemento;
	uint32_t proxima_posicao_vazia;
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
uint32_t chave_anterior = 1;
uint32_t chave_atual = 1;
uint32_t ultima_word_decodificada, penultima_word_decodificada;

double initTime = 0, final_time = 0;

//------------------
// Funções de primos
//------------------
/*unsigned long long int potencia_modulo(unsigned long long int base, unsigned long long int expoente, unsigned long long int modulo)
{
    unsigned long long int resultado = 1;
    base = base % modulo;
 
    while (expoente > 0)
    {
        if (expoente & 1)
            resultado = (resultado*base) % modulo;
 
        expoente = expoente>>1;
        base = (base*base) % modulo;  
    }
    return resultado;
}

bool numero_primo (unsigned long long int numero) {
    int i, j;
    unsigned long long int s, d, a, x;
    bool loop_interrompido = true;

    if (numero == 2)
        return true;
    if (numero % 2 == 0)
        return false;
    
    s = 0;
    d = numero - 1;

    while(d%2 == 0) {
        s++;
        d = d/2;
    }

    for(i = 1; i < numero && i < 8; i++) {
        a = (unsigned long long int) (1 + ((numero - 1) * rand()) / RAND_MAX);

        x = potencia_modulo(a,d,numero);
        if (x != 1 && x != (numero - 1)) {

            loop_interrompido = false;
            for (j = 0; j < s; j++) {
                x = (x * x) % numero;
                if (x == 1)
                    return false;
                else if (x == numero - 1) {
                    loop_interrompido = true;
                    break;
                }
            }

            if (!loop_interrompido)
                return false;
        }
    }

    return true;
}*/

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

void enqueue (fila_espera *fila, uint32_t elemento) {
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


/*int fputc(int c, FILE *f) {
  return(ITM_SendChar(c));
}*/


//------------------------------------------------------------------------------------
// Thread responsavel por encher a fila_chaves com numeros inteiros em ordem crescente
//------------------------------------------------------------------------------------
void gerar_chaves(void const *args){
	uint32_t chave = (mensagem[0] & 0xFFFFFF00) - 0x100;

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
	uint32_t atual;
	int i;
	bool tem_divisor;

	while (!chave_valida) {
		if (fila_primos->full || fila_chaves->empty) {
			osThreadYield();
			continue;
		}
		
		atual = dequeue (fila_chaves);

		//if(atual == 104711)
			//printf("oi! %d", atual);
		
		
		for (i = 2, tem_divisor = false; i <= (int)sqrt((double) atual); i ++) {
			if (atual % i == 0) {
				tem_divisor = true;
				break;
			}
		}

		if (!tem_divisor)
		//if (numero_primo(atual))
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
		
		/*printf("\nMensagem decodificada: %s", mensagem_decodificada);
		printf("\nChave: %d", chave_atual);*/
		decodificado_tail = false;
		fim_teste_ultima = false;
		fim_teste_penultima = false;
		is_mensagem_decodificada = false;
	}
	final_time = ((double)osKernelSysTick())/((double)120);
	//printf("\nTempo total: %d", final_time);
}
osThreadDef (imprime_mensagem, osPriorityNormal, 1, 0);

int main(void) {
	fila_chaves = (fila_espera*) malloc (sizeof (fila_espera));
	fila_primos = (fila_espera*) malloc (sizeof (fila_espera));
	
	criar_fila (fila_primos);
	criar_fila (fila_chaves);

	//srand(time(NULL));

	osKernelInitialize();
	osThreadCreate(osThread(gerar_chaves), NULL);
	osThreadCreate(osThread(verificar_primos), NULL);
	osThreadCreate(osThread(decodificar_tail), NULL);
	osThreadCreate(osThread(testar_penultima_word), NULL);
	osThreadCreate(osThread(testar_ultima_word), NULL);
	osThreadCreate(osThread(decodificar_mensagem), NULL);
	osThreadCreate(osThread(imprime_mensagem), NULL);
	osKernelStart();
	initTime = osKernelSysTick()/120000;
	
	osDelay(osWaitForever);
}

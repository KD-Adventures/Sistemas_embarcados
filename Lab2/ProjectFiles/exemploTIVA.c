/*============================================================================
 *                    LAB 2 - Sistemas Embarcados - S72                      *
 *  Autores: Davi Boberg
 *           Lucas Kaminski de Freitas
 *---------------------------------------------------------------------------*
 * Autores das bibliotecas:
 *      Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 *      Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 *      Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 *      Mariana Carrião        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/
 
#include "cmsis_os.h"

void gerar_chaves(void const *args){
	
}
osThreadDef(gerar_chaves, osPriorityNormal, 1, 0);

void decodificar_tail(void const *args){
	
}
osThreadDef(decodificar_tail, osPriorityNormal, 1, 0);

void verificar_primo(void const *args){
	
}
osThreadDef(verificar_primo, osPriorityNormal, 1, 0);

void testar_penultima_word(void const *args){
	
}
osThreadDef(testar_penultima_word, osPriorityNormal, 1, 0);

void testar_ultima_word(void const *args){
	
}
osThreadDef(testar_ultima_word, osPriorityNormal, 1, 0);

void decodificar_mensagem(void const *args){
	
}
osThreadDef(decodificar_mensagem, osPriorityNormal, 1, 0);

void imprime_mensagem(void const *args){
	
}
osThreadDef(imprime_mensagem, osPriorityNormal, 1, 0);

int main(void) {
	osKernelInitialize();
	
	osThreadCreate(osThread(gerar_chaves), NULL);
	osThreadCreate(osThread(decodificar_tail), NULL);
	osThreadCreate(osThread(verificar_primo), NULL);
	osThreadCreate(osThread(testar_penultima_word), NULL);
	osThreadCreate(osThread(testar_ultima_word), NULL);
	osThreadCreate(osThread(decodificar_mensagem), NULL);
	osThreadCreate(osThread(imprime_mensagem), NULL);
	
	osKernelStart();
	osDelay(osWaitForever);
}

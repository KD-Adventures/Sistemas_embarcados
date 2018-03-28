/*============================================================================
 *                    Exemplos de utilização do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. André Schneider de Oliveira
 *            Universidade Tecnológica Federal do Paraná (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 * 		Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 * 		Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 * 		Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 * 		Mariana Carrião        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/
#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "car.h"
#include "airplane.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "opt.h"
#include "buttons.h"
#include "joy.h"
#include "led.h"

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7

//To print on the screen
tContext sContext;

extern void f_asm(void);
unsigned char ImageMemory[128][128];
int Direction;

enum LoadedImage {Airplane=1, Car};


/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
	joy_init(); 
	button_init();
	rgb_init();
	opt_init();
	led_init();
}

/*----------------------------------------------------------------------------
 *      Switch LED on
 *---------------------------------------------------------------------------*/
void Switch_On (unsigned char led) {
  if (led != LED_CLK) led_on (led);
}
/*----------------------------------------------------------------------------
 *      Switch LED off
 *---------------------------------------------------------------------------*/
void Switch_Off (unsigned char led) {
  if (led != LED_CLK) led_off (led);
}

void loadImageToMemory (const unsigned char* Image) {
	int i, j;
	for (i = 0; i < 128; i++) {
		for (j = 0; j < 128; j++) {
			if (i < 96 && j < 64)
				ImageMemory[i][j] = Image[i + j*96];
			else
				ImageMemory[i][j] = 0;
		}
	}		
}



void printImage (void) {
	int i, j;

	GrContextBackgroundSet(&sContext, ClrBlack);

	for (i = 0; i < 128; i++) {
		for (j = 0; j < 128; j++) {
			if (ImageMemory[i][j] > 130)
				GrContextForegroundSet(&sContext, ClrWhite);
			else
				GrContextForegroundSet(&sContext, ClrBlack);
			GrPixelDraw(&sContext, i, j);
		}
	}
}

	


/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	bool s1_press, s2_press;
	uint16_t direcao_x, direcao_y;	
	enum LoadedImage loadedImage;
	
	//Initializing all peripherals
	init_all();
	loadImageToMemory(carArray);
	loadedImage = Car;

	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	Direction = -1;
	
  while(1){
		printImage();
		
/*  Joystick		*/		
			direcao_x = joy_read_x();
			direcao_y = joy_read_y();
			
			if (loadedImage == Car){
				if (direcao_x < 0x0200)
					Direction=0;
				else if (direcao_x > 0x0E00)
					Direction=1;
			}
			else if (loadedImage == Airplane) {
				if (direcao_y < 0x0200)
					Direction=2;
				else if (direcao_y > 0x0E00)
					Direction=3;
			}
			
			f_asm();


/*	Botoes 	*/			
			s1_press = button_read_s1();
			s2_press = button_read_s2();

			if (s1_press){
				loadImageToMemory(airplaneArray);
				loadedImage = Airplane;
			}
			else if (s2_press){
				loadImageToMemory(carArray);
				loadedImage = Car;
			}
	}	
}

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
#include "rgb.h"
#include "cfaf128x128x16.h"
#include "opt.h"
#include "buttons.h"
#include "joy.h"
#include "led.h"
#include "cmsis_os.h"
#include "Graphics.h"
#include <string.h>
#include "Comunication.h"

#define LEFT 'A'
#define UP 'W'
#define RIGHT 'D'
#define DOWN 'S'

tContext sContext;

// THREADS
void UART (void const *args);
void Menu (void const *args);
void Green_LED (void const *args);
void Red_LED (void const *args);
void Blue_LED (void const *args);

osThreadDef (UART, osPriorityAboveNormal, 1, 0);
osThreadDef (Menu, osPriorityNormal, 1, 0);
osThreadDef (Green_LED, osPriorityNormal, 1, 0);
osThreadDef (Red_LED, osPriorityNormal, 1, 0);
osThreadDef (Blue_LED, osPriorityNormal, 1, 0);

osThreadId UART_id;
osThreadId Menu_id;
osThreadId Green_LED_id;
osThreadId Red_LED_id;
osThreadId Blue_LED_id;

// Message Queues
osMessageQDef(UARTMsgBox, 16, uint32_t);
osMessageQId UARTMsgBox_id;

osMessageQDef(MenuMsgBox, 16, uint32_t);
osMessageQId MenuMsgBox_id;

osMessageQDef(RedLEDMsgBox, 16, uint32_t);
osMessageQId RedLEDMsgBox_id;

osMessageQDef(GreenLEDMsgBox, 16, uint32_t);
osMessageQId GreenLEDMsgBox_id;

osMessageQDef(BlueLEDMsgBox, 16, uint32_t);
osMessageQId BlueLEDMsgBox_id;

//----------------------------------
void UART (void const *args) {
	osEvent event;
	uint32_t received_data;
	
    while (true) {
		event = osMessageGet(UARTMsgBox_id, 100);
		if (event.status == osEventMessage) {
			received_data = event.value.v;
		}
		
        if (received_data > 0) {
            osMessagePut(MenuMsgBox_id, received_data, 0);
            received_data = 0;
        }
    }
}

void handleControls(char received_data, InterfaceMenu * menu) {
	
	switch (received_data) {
		case LEFT:
			previousValue(menu);
			break;
		case RIGHT:
			nextValue(menu);
			break;
		case UP:
			previousEntry(menu);
			break;
		case DOWN:
			nextEntry(menu);
			break;
	}
}

void Menu (void const *args) {
	osEvent event;
	uint32_t received_data;
	uint8_t value_red_led, value_green_led, value_blue_led;
	uint32_t bit_mask = 0x0000ff;
	
	Groups color_groups = loadGroups();
	InterfaceMenu menu = initMenu(&color_groups);
	tContext sContext;
	
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
    while (true) {
		event = osMessageGet(MenuMsgBox_id, 100);
		if (event.status == osEventMessage) {
			received_data = event.value.v;
		}
		
        if (received_data > 0) {
			
//			terminal_clear();
			handleControls(received_data, &menu);
			drawDisplayMenu(&menu, &sContext);
			drawConsoleMenu(&menu);
			//terminal_clear();
			/*
			if (received_data == 'A') {
				sendString(color_groups.colorGroup[BLUE_GROUP].name);
			}
			else if (received_data == 'C') {
				sendString("\033[2J");
			}
			else if (received_data == 'L') {
				sendString("\0338");
			}
			else if (received_data == 'M') {
				sendString("\033[5;10f");
			}
			else if (received_data == 'F') {
				sendString("\033[48;2;155;106;0m");
				GrContextBackgroundSet(&sContext, ClrBlack);
				GrContextForegroundSet(&sContext, ClrWhite);
				rect.i16XMin = 0;
				rect.i16XMax = 20;
				rect.i16YMin = 0;
				rect.i16YMax = 20;
				GrRectFill(&sContext, &rect);
			}
			*/
			value_red_led = (received_data) & bit_mask;
			value_green_led = (received_data >> 8) & bit_mask;
			value_red_led = (received_data >> 16) & bit_mask;
			
			
            osMessagePut(RedLEDMsgBox_id, (uint32_t) received_data, 0);
			osMessagePut(GreenLEDMsgBox_id, (uint32_t) received_data, 0);
			osMessagePut(BlueLEDMsgBox_id, (uint32_t) received_data, 0);
            received_data = 0;
        }
    }
}

void Red_LED (void const *args) {
    osEvent event;
	uint32_t received_data;
	
	while (true) {
		event = osMessageGet(RedLEDMsgBox_id, 100);
		if (event.status == osEventMessage) {
			received_data = event.value.v;
		}
		
        if (received_data > 0) {
			//rgb_write_color(ClrRed);
		}
	}
}

void Green_LED (void const *args) {
    osEvent event;
	uint32_t received_data;
	
	while (true) {
		event = osMessageGet(GreenLEDMsgBox_id, 100);
		if (event.status == osEventMessage) {
			received_data = event.value.v;
		}
		
        if (received_data > 0) {
			rgb_write_color(ClrGreen);
		}
	}
}


void Blue_LED (void const *args) {
    osEvent event;
	uint32_t received_data;
	
	while (true) {
		event = osMessageGet(BlueLEDMsgBox_id, 100);
		if (event.status == osEventMessage) {
			received_data = event.value.v;
		}
		
        if (received_data > 0) {
			//rgb_write_color(ClrBlue);
		}
	}
}

void UART0_Handler() {
    osMessagePut(UARTMsgBox_id, (uint32_t) readUART(), 0);
}

int main (void) {
    IntEnable(INT_UART0);
    initUART();
	rgb_init();
	cfaf128x128x16Init();

    osKernelInitialize();
	
    UART_id = osThreadCreate(osThread(UART), NULL);
    Menu_id = osThreadCreate(osThread(Menu), NULL);
    Green_LED_id = osThreadCreate(osThread(Green_LED), NULL);
    Red_LED_id = osThreadCreate(osThread(Red_LED), NULL);
    Blue_LED_id = osThreadCreate(osThread(Blue_LED), NULL);

	UARTMsgBox_id = osMessageCreate (osMessageQ(UARTMsgBox), NULL);
	MenuMsgBox_id = osMessageCreate (osMessageQ(MenuMsgBox), NULL);
	
	RedLEDMsgBox_id = osMessageCreate (osMessageQ(RedLEDMsgBox), NULL);
	GreenLEDMsgBox_id = osMessageCreate (osMessageQ(GreenLEDMsgBox), NULL);
	BlueLEDMsgBox_id = osMessageCreate (osMessageQ(BlueLEDMsgBox), NULL);

    osKernelStart();
    
    osDelay(osWaitForever);
}
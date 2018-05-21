#include "uart_functions.h"

void initUART() {
	uint32_t *ptr;
	float baud_rate_generator;
	
	//Config instructions:
	//http://www.ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf
	//Page 1172
	
	//Enabling UART module 0
	ptr = (uint32_t*) RCGCUART;
	*ptr = *ptr | 1;
	
	//Providing clock to UART module 0
	ptr = (uint32_t*) RCGCGPIO;
	*ptr = *ptr | 1;
	
	
	
	
	//Falta passos 3, 4 e 5
	
	
	
	
	
	//Disabling UART before configuration
	ptr = (uint32_t*) UARTCTL; 
	*ptr = *ptr & 0xFFFFFFF0; //Clears the "UART Enable" bit
	
	//Setting up a 9600 baud rate
	//Formula used: baud_rate_generator = System clock / (16 * Baud rate)
	baud_rate_generator = (float) SYSTEM_CLOCK / (16.0 * 9600.0); //Não sei onde achar o system clock
	
	//UARTIBRD register receives the integer part of baud_rate_generator
	ptr = (uint32_t*) UARTIBRD;
	*ptr = (uint32_t) baud_rate_generator;
	
	//UARTFBRD register is calculated according to the datasheet formula:
	//UARTFBRD first 6 bits = integer(decimal part * 64 + 0.5)
	ptr = (uint32_t*) UARTFBRD;
	*ptr = 0x3F & (uint32_t) ((baud_rate_generator - (int) baud_rate_generator) * 64 + 0.5);
	
	//The line control register has some additional configuration parameters
	//and must be set after UARTIBRD and UARTFBRD
	ptr = (uint32_t*) UARTLCRH;
	*ptr = WLEN; //Sets the word length to 8 bits.
	
	//UART clock source = system clock
	ptr = (uint32_t*) UARTCC;
	*ptr = 0;
	
	
	
	//Ver se é necessário mexer no UARTDMACTL
	
	
	
	//Enabling UART
	ptr = (uint32_t*) UARTCTL; 
	*ptr = *ptr | UARTEN; //Sets the "UART Enable" bit
}

void sendUART(uint8_t data) {
	uint32_t *ptr;
	
	//Waits until BUSY flag is turned off
	ptr = (uint32_t*) UARTFR;
	while((*ptr & BUSY) > 0);
	
	//Sends data through the data register
	ptr = (uint32_t*) UARTDR;
	*ptr = (*ptr & 0xFFFFFF00) | data;
}
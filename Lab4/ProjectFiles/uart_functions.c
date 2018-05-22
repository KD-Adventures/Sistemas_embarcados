#include "uart_functions.h"

uint8_t global = 0;

void initUART() {
    /******************************************************
     * Configuration instructions:                        *
     * http://www.ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf *
     * Page 1172                                          *
     ******************************************************/    

    /******************************************
     * SYSTEM CONTROL REGISTERS CONFIGURATION *
     ******************************************/
    reg(RCGCUART) |= 1; // Enabling UART module 0
    reg(RCGCGPIO) |= 1; // Providing clock to UART module 0


    /********************************
     * GPIO REGISTERS CONFIGURATION *
     ********************************/
    reg(GPIODEN_PORT_A) |= 0x00000003;     // Enabling pins 0 and 1 of GPIO Port A
    reg(GPIOAFSEL_PORT_A) |= 0x00000003;   // Enabling alternate functions of pins 0 and 1 of GPIO Port A

    // Setting the alternate function of those pins to UART
    reg(GPIOPCTL_PORT_A) = (reg(GPIOPCTL_PORT_A) & 0xFFFFFF00) | 0x00000011;


    /********************************
     * UART REGISTERS CONFIGURATION *
     ********************************/
    reg(UARTCTL) = 0;    // Disabling UART before configuration
    reg(UARTIM) |= RXIM; // Enabling receive interrupt

    /* Setting up a 9600 baud rate                                                *
     * Formula used: baud_rate_generator = System clock 16 MHz / (16 * Baud rate) *
     * UARTIBRD register receives the integer part of baud_rate_generator         *
     * UARTFBRD register is calculated according to the datasheet formula:        *
     *            First 6 bits = integer(decimal part * 64 + 0.5)                 *
     * UARTLCRH must be set after UARTIBRD and UARTFBRD                           */
    reg(UARTIBRD) = (reg(UARTIBRD) & 0xFFFF0000) | 0x0068;
    reg(UARTFBRD) = (reg(UARTFBRD) & 0xFFFFFFC0) | 0x0B;
    reg(UARTLCRH) = 0x00000060; //Setting the word length to 8 bits.

    reg(UARTCTL) = UARTEN | RXE | TXE; //Enabling UART
}

void sendUART(uint8_t data) {
    uint32_t i;
    while(reg(UARTFR) & BUSY); //Waits until BUSY flag is turned off
    reg(UARTDR) = data; //Sends data through the data register
    for (i = 0; i < 10000; i++);
}

uint8_t readUART() {
	return (uint8_t) reg(UARTDR);
}
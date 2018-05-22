#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS

#include "TM4C129.h"
#include <stdbool.h>
#include "inc\hw_ints.h"
#include "driverlib\interrupt.h"

//Register access macro
#define reg(x) *(uint32_t*) x


/*************************
 * GPIO PORT A REGISTERS *
 *************************/
#define GPIO_PORT_A_BASE_ADDR 0x40058000

//Alternate Function Select Register
#define GPIOAFSEL_PORT_A (GPIO_PORT_A_BASE_ADDR + 0x420)

//GPIO Digital Enable
#define GPIODEN_PORT_A (GPIO_PORT_A_BASE_ADDR + 0x51C)

//GPIO Port Control
#define GPIOPCTL_PORT_A (GPIO_PORT_A_BASE_ADDR + 0x52C)


/****************************
 * SYSTEM CONTROL REGISTERS *
 ****************************/
#define SYSTEM_CONTROL_BASE_ADDR 0x400FE000

//Run Mode Clock Gating Control
#define RCGCGPIO (SYSTEM_CONTROL_BASE_ADDR + 0x608)

//UART Run Mode Clock Gating Control
#define RCGCUART (SYSTEM_CONTROL_BASE_ADDR + 0x618)


/******************
 * UART REGISTERS *
 ******************/
#define UART0_BASE_ADDR 0x4000C000

//Data register
#define UARTDR UART0_BASE_ADDR

//Flag register
#define UARTFR (UART0_BASE_ADDR + 0x018)
#define BUSY (1<<3)

//Baud-rate generator registers
#define UARTIBRD (UART0_BASE_ADDR + 0x024)
#define UARTFBRD (UART0_BASE_ADDR + 0x028)

//Line control register
#define UARTLCRH (UART0_BASE_ADDR + 0x02C)
#define WLEN (3<<5);

//Control register
#define UARTCTL (UART0_BASE_ADDR + 0x030)
#define RXE (1<<9)
#define TXE (1<<8)
#define UARTEN 1

//Interrupt mask register
#define UARTIM (UART0_BASE_ADDR + 0x038)
#define RXIM (1<<4)


/*************
 * FUNCTIONS *
 *************/
// Configures all the necessary registers
void initUART();

// Sends an 8-bit char through the UART
void sendUART(uint8_t data);

// UART receive interrupt handler
void UART0_Handler();

// Global variable
extern bool global;

#endif
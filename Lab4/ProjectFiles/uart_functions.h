#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS

#include "TM4C129.h"

//---------------------------------------------------
//SYSTEM CONTROL REGISTERS
#define SYSTEM_CONTROL_BASE_ADDR 0x400FE000

//GPIO Alternate Function Select
#define GPIOAFSEL (SYSTEM_CONTROL_BASE_ADDR + 0x420)

//Run Mode Clock Gating Control
#define RCGCGPIO (SYSTEM_CONTROL_BASE_ADDR + 0x608)

//UART Run Mode Clock Gating Control
#define RCGCUART (SYSTEM_CONTROL_BASE_ADDR + 0x618)

//---------------------------------------------------
//UART REGISTERS
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

//Clock Configuration
#define UARTCC (UART0_BASE_ADDR + 0xFC8)

void initUART();
void sendUART(uint8_t data);

#endif
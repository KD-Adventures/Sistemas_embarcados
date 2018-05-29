#ifndef COMUNICATION_H
#define COMUNICATION_H

#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include "Colors.h"
#include "uart_functions.h"
#include <string.h>


void sendString(char* string);

#endif
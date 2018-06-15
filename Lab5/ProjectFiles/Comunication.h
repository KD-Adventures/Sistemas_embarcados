#ifndef COMUNICATION_H
#define COMUNICATION_H

#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include "uart_functions.h"
#include <string.h>
#include "Utils.h"

void sendString(char* string);

#endif
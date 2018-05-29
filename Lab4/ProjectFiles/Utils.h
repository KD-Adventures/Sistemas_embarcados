#ifndef UTILS_H
#define UTILS_H

#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include <string.h>

void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros);

#endif
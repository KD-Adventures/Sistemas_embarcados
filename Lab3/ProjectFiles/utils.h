#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros);
uint32_t difference(uint32_t value1, uint32_t value2);

#endif
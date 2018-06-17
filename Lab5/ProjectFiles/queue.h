#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

void left_shift(void* queue[], int size, int shift);
void left_shift_from_index(void* queue[], int size, int shift, int index);

//void right_shift(void* queue[], int size, int shift);

void* get_first_element(void* queue[], int *size);
void* get_element(void* queue[], int *size, int index);
void push_element(void* queue[], int max_size, void* element, int *size);

#endif
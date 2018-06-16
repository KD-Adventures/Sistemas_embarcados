#ifndef QUEUE_H
#define QUEUE_H

void left_shift(void* queue[], int size, int shift);
void right_shift(void* queue[], int size, int shift);

void* get_first_element(void* queue[], int *size);
void put_element(void* queue[], int max_size, void* element, int *index);

#endif
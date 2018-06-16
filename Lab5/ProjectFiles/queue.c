#include "queue.h"

void left_shift(void* queue[], int size, int shift) {
	int i;
	
	for (i = 0; i < size - 1; i++) {
		queue[i] = queue[i + 1];
	}
}

void right_shift(void* queue[], int size, int shift) {

}

void* get_first_element(void* queue[], int *size) {
	void* value = queue[0];
	left_shift(queue, *size, 1);
	(*size)--;
	
	return value;
}

void put_element(void* queue[], int max_size, void* element, int *index) {
	if (*index >= max_size || *index < 0) {
		return;
	}
	queue[*index] = element;
	(*index)++;
}
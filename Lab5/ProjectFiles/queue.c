#include "queue.h"

void left_shift(void* queue[], int size, int shift) {
	left_shift_from_index(queue, size, shift, 0);
}

void left_shift_from_index(void* queue[], int size, int shift, int index) {
	int i;
	
	if (size <= 0) 
		return;
	if (index < 0 || index >= size)
		return;
	
	for (i = index; i < size - 1; i++) {
		queue[i] = queue[i + 1];
	}
	queue[size-1] = NULL; 
}

//void right_shift(void* queue[], int size, int shift) {}

void* get_first_element(void* queue[], int *size) {
	return get_element(queue, size, 0);
}

void* get_element(void* queue[], int *size, int index) {
	void* value;
	
	if (*size <= 0) {
		return 0;
	}
	if (index >= *size || index < 0) {
		return 0;
	}
	
	value = queue[index];
	left_shift_from_index(queue, *size, 1, index);
	(*size)--;
	
	return value;
}

void push_element(void* queue[], int max_size, void* element, int *size) {
	if (*size >= max_size || *size < 0) {
		return;
	}
	queue[*size] = element;
	(*size)++;
}
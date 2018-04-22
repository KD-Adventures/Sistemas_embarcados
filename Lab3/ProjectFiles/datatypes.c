#include "datatypes.h"
#include <stdlib.h>

Image_matrix* new_matrix_image(uint32_t height, uint32_t width) {
	
	int i = 0, j=0;
	Image_matrix* new_matrix = (Image_matrix*)malloc(sizeof(Image_matrix));
	new_matrix->height = height;
	new_matrix->width = width;
	
	new_matrix->values = (uint32_t**)malloc(sizeof(uint32_t*)*(new_matrix->height));
	for(i = 0; i < new_matrix->height; i++) {
		new_matrix->values[i] = (uint32_t*)malloc(sizeof(uint32_t)*(new_matrix->width));
		
		for(j = 0; j < new_matrix->width; j++) {
			new_matrix->values[i][j] = 0;
		}
	}
	
	return new_matrix;
}

void delete_matrix_image(Image_matrix* image) {
	
	int i = 0;
	if(image == NULL)
		return;
	
	for(i = 0; i < image->height; i++) {
		free(image->values[i]);
	}
	free(image->values);
	free(image);
	
	return;
}
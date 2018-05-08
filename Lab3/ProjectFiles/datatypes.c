#include "datatypes.h"

void swap(uint32_t* x1, uint32_t* x2) {
	uint32_t tmp = *x1;
	*x1 = *x2;
	*x2 = tmp;
}

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

void delete_matrix_image(Image_matrix** image) {

	int i = 0;
	if(*image == NULL)
		return;

	for(i = 0; i < (*image)->height; i++) {
		free((*image)->values[i]);
	}
	free((*image)->values);
	free(*image);

	return;
}

void invert_image_axis_x(Image_matrix* image_display) {
	int i = 0;
	int j = 0;
	for(i = 0; i < image_display->width; i++) {
		for( j = 0; j < image_display->height/2; j++) {
			swap(&image_display->values[j][i], &image_display->values[image_display->height - j - 1][i]);
		}
	}
}

void invert_image_axis_y(Image_matrix* image_display) {
	int i = 0;
	int j = 0;
	for(i = 0; i < image_display->height; i++) {
		for( j = 0; j < image_display->width/2; j++) {
			swap(&image_display->values[i][j], &image_display->values[i][image_display->width - j -1]);
		}
	}
}


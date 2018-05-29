#ifndef TERMINAL_H
#define TERMINAL_H

#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include "Comunication.h"
#include "Utils.h"

void terminal_clear();

void goto_xy(uint32_t x, uint32_t y);

void background_color(uint32_t rgb);
void background_color_r_g_b(uint32_t reb, uint32_t green, uint32_t blue);
void foreground_color(uint32_t rgb);
void foreground_color_r_g_b(uint32_t red, uint32_t green, uint32_t blue);

void draw_square(uint32_t x_min, uint32_t x_max, uint32_t y_min, uint32_t y_max, uint32_t color);


#endif
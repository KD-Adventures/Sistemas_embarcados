#ifndef GANTT_H
#define GANTT_H

#include "Communication.h"

void gantt_generate_header();
void gantt_thread_enter(char *thread_name, int enter_time);
void gantt_thread_exit(char *thread_name, int exit_time);

#endif
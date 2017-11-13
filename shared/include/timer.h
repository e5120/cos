#pragma once
#include "timer_s.h"
#include "mult_task.h"
#include "interrupt.h"

void init_pit(void);
void interrupt_handler20(int *esp);
void init_timer(TIMER* timer, FIFO32* fifo, int data);
void settimer(TIMER* timer, unsigned int timeout);
TIMER* timer_alloc(void);
void timer_free(TIMER* timer);


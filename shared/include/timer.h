#pragma once
#include "timer_s.h"
#include "mult_task.h"
#include "interrupt.h"
#include "fifo.h"

#define PIT_CTRL 0x0043
#define PIT_CNT  0x0040

#define TIMER_FREE  0
#define TIMER_ALLOC 1
#define TIMER_USED  2

extern TIMER* task_timer;
void init_pit(void);
void interrupt_handler20(int *esp);
void init_timer(TIMER* timer, FIFO32* fifo, int data);
void settimer(TIMER* timer, unsigned int timeout);
TIMER* timer_alloc(void);
void timer_free(TIMER* timer);


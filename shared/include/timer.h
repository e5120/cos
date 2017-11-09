#pragma once
#include "asmfunc_def.h"
#include "interrupt.h"
#include "fifo.h"

#define PIT_CTRL 0x0043
#define PIT_CNT  0x0040

#define MAX_TIMER   500
#define TIMER_FREE  0
#define TIMER_ALLOC 1
#define TIMER_USED  2

typedef struct TIMER{
    struct TIMER* next;
    unsigned int timeout;
    unsigned int flags;
    FIFO32 *fifo;
    int data;
}TIMER;

typedef struct TIMER_CONTROL{
    unsigned int count;
    unsigned int next;
    TIMER *timer;
    TIMER timer0[MAX_TIMER];
}TIMER_CTL;

TIMER_CTL timer_ctl;

void init_pit(void);
void interrupt_handler20(int *esp);
void init_timer(TIMER* timer, FIFO32* fifo, int data);
void settimer(TIMER* timer, unsigned int timeout);
TIMER* timer_alloc(void);
void timer_free(TIMER* timer);


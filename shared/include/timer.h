#pragma once
#include "asmfunc_def.h"
#include "interrupt.h"
#include "fifo.h"

#define PIT_CTRL 0x0043
#define PIT_CNT 0x0040

typedef struct TIMER_CONTROL{
    unsigned int count;
    unsigned int timeout;
    FIFO *fifo;
    unsigned char data;
}TIMER_CTL;

void init_pit(void);
void interrupt_handler20(int *esp);
void settimer(unsigned int timeout, FIFO* fifo, unsigned char data);
#pragma once
#include "fifo_s.h"

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

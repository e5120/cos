#pragma once
#include "fifo_s.h"

#define MAX_TIMER   500

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

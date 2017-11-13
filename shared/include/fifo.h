#pragma once
#include "fifo_s.h"
#include "mult_task.h"
#define FLAGS_OVERRUN 0x0001

void init_fifo32(FIFO32* fifo, int size, int *buf, struct TASK* task);
int put_fifo32(FIFO32* fifo, int data);
int get_fifo32(FIFO32* fifo);
int fifo_status32(FIFO32* fifo);


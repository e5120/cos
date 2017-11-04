#pragma once
#define FLAGS_OVERRUN 0x0001

typedef struct FIFO{
    unsigned char *buf;
    int r_flag;
    int w_flag;
    int size;
    int free;
    int flags;
}FIFO;

void init_fifo(FIFO* fifo, int size, unsigned char *buf);
int put_fifo(FIFO* fifo, unsigned char data);
int get_fifo(FIFO* fifo);
int fifo_status(FIFO* fifo);
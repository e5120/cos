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

typedef struct FIFO32{
    int *buf;
    int r_flag;
    int w_flag;
    int size;
    int free;
    int flags;
}FIFO32;


void init_fifo(FIFO* fifo, int size, unsigned char *buf);
int put_fifo(FIFO* fifo, unsigned char data);
int get_fifo(FIFO* fifo);
int fifo_status(FIFO* fifo);

void init_fifo32(FIFO32* fifo, int size, int *buf);
int put_fifo32(FIFO32* fifo, int data);
int get_fifo32(FIFO32* fifo);
int fifo_status32(FIFO32* fifo);


#pragma once
// 16bitのバッファを持つFIFO
typedef struct FIFO{
    unsigned char *buf;
    int r_flag;
    int w_flag;
    int size;
    int free;
    int flags;
}FIFO;

// 32bitのバッファを持つFIFO
typedef struct FIFO32{
    int *buf;
    int r_flag;
    int w_flag;
    int size;
    int free;
    int flags;
    struct TASK *task;
}FIFO32;

#include "../include/fifo.h"

void init_fifo(FIFO* fifo, int size, unsigned char *buf){
  fifo->size = size;
  fifo->buf = buf;
  fifo->free = size;
  fifo->flags = 0;
  fifo->r_flag = 0;
  fifo->w_flag = 0;
  return;
}

int put_fifo(FIFO* fifo, unsigned char data){
  if(!fifo->free){
    fifo->flags |= FLAGS_OVERRUN;
    return -1;
  }
  fifo->buf[fifo->w_flag] = data;
  ++fifo->w_flag;
  if(fifo->w_flag == fifo->size){
    fifo->w_flag = 0;
  }
  --fifo->free;
  return 0;
}

int get_fifo(FIFO* fifo){
  int data;
  if(fifo->free == fifo->size){
    return -1;
  }
  data = fifo->buf[fifo->r_flag];
  ++fifo->r_flag;
  if(fifo->r_flag == fifo->size){
    fifo->r_flag = 0;
  }
  ++fifo->free;
  return data;
}

int fifo_status(FIFO* fifo){
  return fifo->size - fifo->free;
}
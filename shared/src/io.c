#include "../include/io.h"

FIFO32 *keyfifo, *mousefifo;
int keydata, mousedata;

// キーボードからの割り込み
void interrupt_handler21(int *esp){
  int data;
  io_out8(PIC0_OCW2, 0x61);   //IRQ-01受付完了をPICに通知
  data = io_in8(PORT_KEYDATA);
  put_fifo32(keyfifo, data + keydata);
  return;
}

void wait_KBC_sendready(void){
  while(1){
    if((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0){
      break;
    }
  }
  return;
}

void init_keyboard(FIFO32* fifo, int data){
  keyfifo = fifo;
  keydata = data;
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDATA, KBC_MODE);
  return;
}

// マウスからの割り込み
void interrupt_handler2c(int *esp){
  int data;
  io_out8(PIC1_OCW2, 0x64);
  io_out8(PIC0_OCW2, 0x62);
  data = io_in8(PORT_KEYDATA);
  put_fifo32(mousefifo, data + mousedata);
  return;
}

void enable_mouse(FIFO32* fifo, int data, MOUSE_DEC* mdec){
  mousefifo = fifo;
  mousedata = data;
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDATA, MOUSECMD_ENABLE);
  mdec->phase = 0;
  return;
}

int mouse_decode(MOUSE_DEC* mdec, unsigned char data){
  if(mdec->phase == 0){
    // 0xfaは要らないので捨てる
    if(data == 0xfa){
      mdec->phase  = 1;
    }
    return 0;
  }
  if(mdec->phase == 1){
    // 正しいデータか判定
    if((data & 0xc8) == 0x08){
      mdec->buf[0] = data;
      mdec->phase = 2;
    }
    return 0;
  }
  if(mdec->phase == 2){
    mdec->buf[1] = data;
    mdec->phase = 3;
    return 0;
  }
  if(mdec->phase == 3){
    mdec->buf[2] = data;
    mdec->phase = 1;
    mdec->btn = mdec->buf[0] & 0x07;
    mdec->x = mdec->buf[1];
    mdec->y = mdec->buf[2];
    if((mdec->buf[0] & 0x10) != 0){
      mdec->x |= 0xffffff00;
    }
    if((mdec->buf[0] & 0x20) != 0){
      mdec->y |= 0xffffff00;
    }
    mdec->y = -mdec->y;
    return 1;
  }
  return -1;
}


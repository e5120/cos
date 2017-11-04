#include "../include/interrupt.h"

#define PORT_KEYDATA  0x0060

void init_pic(void){
  io_out8(PIC0_IMR, 0xff);    // 全割り込み受付不可
  io_out8(PIC1_IMR, 0xff);    // 全割り込み受付不可

  io_out8(PIC0_ICW1, 0x11);   // エッジトリガモード
  io_out8(PIC0_ICW2, 0x20);   // IRQ0-7はINT20-27で受ける
  io_out8(PIC0_ICW3, 1 << 2); // PIC1はIRQ2で接続
  io_out8(PIC0_ICW4, 0x01);   // ノンバッファモード

  io_out8(PIC1_ICW1, 0x11);   // エッジトリガモード
  io_out8(PIC1_ICW2, 0x28);   // IRQ8-15はINT28-2fで受ける
  io_out8(PIC1_ICW3, 2);      // PIC1はIRQ2で接続
  io_out8(PIC1_ICW4, 0x01);   // ノンバッファモード

  io_out8(PIC0_IMR, 0xfb);    // 11111011 PIC1以外は全て禁止
  io_out8(PIC1_IMR, 0xff);    // 11111111 全割り込み受付不可

  return;
}

FIFO k_fifo;
// キーボードからの割り込み
void interrupt_handler21(int *esp){
  unsigned char data;
  io_out8(PIC0_OCW2, 0x61);   //IRQ-01受付完了をPICに通知
  data = io_in8(PORT_KEYDATA);
  put_fifo(&k_fifo, data);
  return;
}

FIFO m_fifo;
// マウスからの割り込み
void interrupt_handler2c(int *esp){
  unsigned char data;
  io_out8(PIC1_OCW2, 0x64);
  io_out8(PIC0_OCW2, 0x62);
  data = io_in8(PORT_KEYDATA);
  put_fifo(&m_fifo, data);
  return;
}

void interrupt_handler27(int *esp){
  io_out8(PIC0_OCW2, 0x67);   // IRQ-07受付完了をPICに通知
  return;
}
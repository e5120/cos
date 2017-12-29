#include "../include/interrupt.h"


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

void interrupt_handler27(int *esp){
  io_out8(PIC0_OCW2, 0x67);   // IRQ-07受付完了をPICに通知
  return;
}

int interrupt_handler0d(int *esp){
  CONSOLE *cons = (CONSOLE*) *((int*) 0x0fec);
  TASK *task = task_now();
  cons_putstr(cons, "\nINT 0D :\n General Protected Exception.\n");
  return &(task->tss.esp0);
}
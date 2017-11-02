#include "../include/bootpack.h"
#include "../include/interrupt.h"
#include "../include/address.h"
#include "../include/graphic.h"
#include "../include/color_set.h"

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

// キーボードからの割り込み
void interrupt_handler21(int *esp){
  BOOT_INFO *binfo = (BOOT_INFO*)ADDR_BOOTINFO;
  draw_rectangle((unsigned char*)binfo->vram, binfo->screen_x, COLOR_000000, 0, 0, 32*8 - 1, 15);
  put_string(binfo->vram, binfo->screen_x, 0, 0, COLOR_FFFFFF, "INT 21 (IRQ-1):PS/2 keyboard");
  for(;;){
    io_hlt();
  }
}

// マウスからの割り込み
void interrupt_handler2c(int *esp){
  BOOT_INFO *binfo = (BOOT_INFO*)ADDR_BOOTINFO;
  draw_rectangle((unsigned char*)binfo->vram, binfo->screen_x, COLOR_000000, 0, 0, 32*8 - 1, 15);
  put_string(binfo->vram, binfo->screen_x, 0, 0, COLOR_FFFFFF, "INT 2C (IRQ-2):PS/2 mouse");
  for(;;){
    io_hlt();
  }
}

void interrupt_handler27(int *esp){
  io_out8(PIC0_OCW2, 0x67);   // IRQ-07受付完了をPICに通知
  return;
}
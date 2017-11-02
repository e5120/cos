#include "../include/bootpack.h"
#include "../include/graphic.h"
#include "../include/macro.h"
#include "../include/dsctbl.h"
#include "../include/color_set.h"
#include "../include/address.h"
#include "../include/interrupt.h"

void HariMain(void){
  char* vram_addr;
  char mcursor[16*16];
  int screen_x, screen_y;
  int back_color;
  BOOT_INFO*  binfo;

  binfo = (BOOT_INFO*)ADDR_BOOTINFO;   // asmhead.asmでのBOOT_INFO先頭番地
  screen_x = (*binfo).screen_x;
  screen_y = (*binfo).screen_y;
  vram_addr = (*binfo).vram;    // VRAMの先頭アドレス

  init_gdtidt();
  init_pic();
  io_sti();

  init_palette();

  // 背景色設定
  back_color = COLOR_008484;
  set_background((unsigned char*)vram_addr, screen_x, screen_y, back_color);
  init_desktop((unsigned char*)vram_addr, screen_x, screen_y);
  init_mouse_cursor(mcursor, back_color);
  put_block(vram_addr, screen_x, 16, 16, 100, 100, mcursor, 16);

  io_out8(PIC0_IMR, 0xf9);    // PIC1とキーボードを許可
  io_out8(PIC1_IMR, 0xef);    // マウスを許可

  while(1){
    io_hlt();
  }
}





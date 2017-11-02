#include "../include/bootpack.h"
#include "../include/graphic.h"
#include "../include/macro.h"
#include "../include/dsctbl.h"
#include "../include/color_set.h"

void HariMain(void){
  char* vram_addr;
  char mcursor[16*16];
  int screen_x, screen_y;
  int back_color;
  BOOT_INFO*  binfo;

  binfo = (BOOT_INFO*)0x0ff0;   // asmhead.asmでのBOOT_INFO先頭番地
  screen_x = (*binfo).screen_x;
  screen_y = (*binfo).screen_y;
  vram_addr = (*binfo).vram;    // VRAMの先頭アドレス

  // 背景色設定
  back_color = COLOR_008484;
  set_background((unsigned char*)vram_addr, screen_x, screen_y, back_color);
  init_palette();
  init_desktop((unsigned char*)vram_addr, screen_x, screen_y);
  init_mouse_cursor(mcursor, back_color);

  put_string(vram_addr, screen_x, 8, 8, COLOR_FFFFFF, "hello world");
  put_string(vram_addr, screen_x, 8, 26, COLOR_000000, "1 + 2 * 3 / 4 = x");

  char* str = "a";
  lsprintf(str, "screen_width = %d", screen_x);
  put_string(vram_addr, screen_x, 8, 100, COLOR_000000, str);
  put_block(vram_addr, screen_x, 16, 16, 100, 100, mcursor, 16);

  while(1){
    io_hlt();
  }
}





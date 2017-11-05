#include "../include/bootpack.h"

extern FIFO k_fifo, m_fifo;

void HariMain(void){
  char mcursor[16*16], str[STR_MAX_BUF], keybuf[KEY_MAX_BUF], mousebuf[MOUSE_MAX_BUF];
  int mouse_x, mouse_y;
  int back_color;
  int i;
  BOOT_INFO* binfo = (BOOT_INFO*)ADDR_BOOTINFO;   // asmhead.asmでのBOOT_INFO先頭番地
  MOUSE_DEC mdec;

  init_gdtidt();
  init_pic();
  io_sti();

  init_fifo(&k_fifo, KEY_MAX_BUF, keybuf);
  init_fifo(&m_fifo, MOUSE_MAX_BUF, mousebuf);

  io_out8(PIC0_IMR, 0xf9);    // PIC1とキーボードを許可
  io_out8(PIC1_IMR, 0xef);    // マウスを許可
  init_keyboard();

  init_palette();
  // 背景色設定
  back_color = COLOR_008484;
  // 背景出力
  set_background(binfo->vram, binfo->screen_x, binfo->screen_y, back_color);
  init_desktop(binfo->vram, binfo->screen_x, binfo->screen_y);

  // マウスカーソルの初期化・出力・割り込み許可
  init_mouse_cursor(mcursor, back_color);
  mouse_x = mouse_y = 100;
  put_block(binfo->vram, binfo->screen_x, 16, 16, mouse_x, mouse_y, mcursor, 16);
  enable_mouse(&mdec);

  while(1){
    io_cli();
    if (!(fifo_status(&k_fifo) + fifo_status(&m_fifo))){
      io_stihlt();
    }
    else{
      if(fifo_status(&k_fifo)){
        i = get_fifo(&k_fifo);
        io_sti();
        lsprintf(str, "%X", i);
        draw_rectangle(binfo->vram, binfo->screen_x, back_color, 0, 16, 15, 15);
        put_string(binfo->vram, binfo->screen_x, 0, 16, COLOR_FFFFFF, str);
      }
      else if(fifo_status(&m_fifo)){
        i = get_fifo(&m_fifo);
        io_sti();
        if(mouse_decode(&mdec, i) == 1){
          lsprintf(str, "[lcr %d %d]", mdec.x, mdec.y);
          if(mdec.btn & 0x01){
            str[1] = 'L';
          }
          else if(mdec.btn & 0x02){
            str[3] = 'R';
          }
          else if(mdec.btn & 0x04){
            str[2] = 'C';
          }
          draw_rectangle(binfo->vram, binfo->screen_x, back_color, 32, 16, 15*8 - 1, 16);
          put_string(binfo->vram, binfo->screen_x, 32, 16, COLOR_FFFFFF, str);

          // 現在表示されているマウスを消す
          draw_rectangle(binfo->vram, binfo->screen_x,
                          back_color, mouse_x, mouse_y, 16, 16);
          mouse_x += mdec.x;
          mouse_y += mdec.y;
          if(mouse_x < 0){
            mouse_x = 0;
          }
          else if(mouse_x > binfo->screen_x - 16){
            mouse_x = binfo->screen_x - 16;
          }
          if(mouse_y < 0){
            mouse_y = 0;
          }
          else if(mouse_y > binfo->screen_y - 16){
            mouse_y = binfo->screen_y - 16;
          }
          put_block(binfo->vram, binfo->screen_x, 16, 16, mouse_x, mouse_y, mcursor, 16);
        }
      }
    }
  }
}





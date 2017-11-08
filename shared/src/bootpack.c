#include "../include/bootpack.h"

extern FIFO k_fifo, m_fifo;

void HariMain(void){
  char str[STR_MAX_BUF], keybuf[KEY_MAX_BUF], mousebuf[MOUSE_MAX_BUF];
  int mouse_x, mouse_y, i, counter = 0;
  int back_color;

  BOOT_INFO* binfo = (BOOT_INFO*)ADDR_BOOTINFO;   // asmhead.asmでのBOOT_INFO先頭番地
  MOUSE_DEC mdec;

  unsigned int memtotal;
  MEM_MAN* memman = (MEM_MAN*)MEMMANAGER_ADDR;

  LAYER_CTL* layer_ctl;
  LAYER *layer_back, *layer_mouse, *layer_window;
  unsigned char *buf_back, buf_mouse[MOUSE_MAX_BUF], *buf_window;

  init_gdtidt();
  init_pic();
  io_sti();

  init_fifo(&k_fifo, KEY_MAX_BUF, keybuf);
  init_fifo(&m_fifo, MOUSE_MAX_BUF, mousebuf);

  io_out8(PIC0_IMR, 0xf9);    // PIC1とキーボードを許可
  io_out8(PIC1_IMR, 0xef);    // マウスを許可

  init_keyboard();
  enable_mouse(&mdec);
  // メモリチェック
  i = memtest(0x00400000, 0xbfffffff) / (1024 * 1024);
  memtotal = memtest(0x00400000, 0xbfffffff);
  memory_manage_init(memman);
  memory_manage_free(memman, 0x00001000, 0x0009e000);
  memory_manage_free(memman, 0x00400000, memtotal - 0x00400000);

  init_palette();
  // 背景色設定
  back_color = COLOR_008484;
  // レイヤー設定
  layer_ctl = layer_control_init(memman, (unsigned char*)binfo->vram, binfo->screen_x, binfo->screen_y);
  layer_back = layer_alloc(layer_ctl);
  layer_mouse = layer_alloc(layer_ctl);
  layer_window = layer_alloc(layer_ctl);
  buf_back = (unsigned char*)memory_manage_alloc_4k(memman, binfo->screen_x * binfo->screen_y);
  buf_window = (unsigned char*)memory_manage_alloc_4k(memman, 160 * 68);
  layer_setbuf(layer_back, buf_back, binfo->screen_x, binfo->screen_y, -1);
  layer_setbuf(layer_mouse, buf_mouse, 16, 16, 99);
  layer_setbuf(layer_window, buf_window, 160, 68, -1);
  init_desktop((char*)buf_back, binfo->screen_x, binfo->screen_y, back_color);
  init_mouse_cursor((char*)buf_mouse, 99);
  make_window(buf_window, 160, 68, "counter");
  layer_slide(layer_back, 0, 0);
  mouse_x = (binfo->screen_x - 16) / 2;
  mouse_y = (binfo->screen_y - 28 - 16) / 2;
  layer_slide(layer_mouse, mouse_x, mouse_y);
  layer_slide(layer_window, 80, 72);

  layer_updown(layer_back, 0);
  layer_updown(layer_window, 1);
  layer_updown(layer_mouse, 2);

  lsprintf(str, "memory : %dMB    free:%dKB", memtotal/(1024*1024), memory_manage_total(memman)/1024);
  put_string((char*)buf_back, binfo->screen_x, 0, 32, COLOR_FFFFFF, str);
  layer_refresh(layer_back, 0, 0, binfo->screen_x, 48);

  while(1){
    ++counter;
    lsprintf(str, "%d", counter);
    draw_rectangle(buf_window, 160, COLOR_C6C6C6, 40, 28, 80, 16);
    put_string(buf_window, 160, 40, 28, COLOR_000000, str);
    layer_refresh(layer_window, 40, 28, 120, 44);

    io_cli();
    if (!(fifo_status(&k_fifo) + fifo_status(&m_fifo))){
      io_sti();
    }
    else{
      if(fifo_status(&k_fifo)){
        i = get_fifo(&k_fifo);
        io_sti();
        lsprintf(str, "%X", i);
        draw_rectangle((char*)buf_back, binfo->screen_x, back_color, 0, 16, 15, 15);
        put_string((char*)buf_back, binfo->screen_x, 0, 16, COLOR_FFFFFF, str);
        layer_refresh(layer_back, 0, 16, 16, 32);
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
          draw_rectangle((char*)buf_back, binfo->screen_x, back_color, 32, 16, 15*8 - 1, 16);
          put_string((char*)buf_back, binfo->screen_x, 32, 16, COLOR_FFFFFF, str);
          layer_refresh(layer_back, 32, 16, 32 + 15 * 8, 32);
          mouse_x += mdec.x;
          mouse_y += mdec.y;
          if(mouse_x < 0){
            mouse_x = 0;
          }
          else if(mouse_x > binfo->screen_x - 1){
            mouse_x = binfo->screen_x - 1;
          }
          if(mouse_y < 0){
            mouse_y = 0;
          }
          else if(mouse_y > binfo->screen_y - 1){
            mouse_y = binfo->screen_y - 1;
          }
          layer_slide(layer_mouse, mouse_x, mouse_y);
        }
      }
    }
  }
}





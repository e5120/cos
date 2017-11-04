#include "../include/bootpack.h"

extern FIFO k_fifo, m_fifo;

void HariMain(void){
  char* vram_addr;
  char mcursor[16*16], str[MAX_BUF], keybuf[MAX_BUF], mousebuf[128];
  int screen_x, screen_y;
  int back_color;
  int i;
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

  init_fifo(&k_fifo, MAX_BUF, keybuf);
  init_fifo(&m_fifo, 128, mousebuf);

  io_out8(PIC0_IMR, 0xf9);    // PIC1とキーボードを許可
  io_out8(PIC1_IMR, 0xef);    // マウスを許可
  init_keyboard();
  enable_mouse();

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
        draw_rectangle((unsigned char*)vram_addr, screen_x, back_color, 0, 16, 15, 15);
        put_string(vram_addr, screen_x, 0, 16, COLOR_FFFFFF, str);
      }
      else if(fifo_status(&m_fifo)){
        i = get_fifo(&m_fifo);
        io_sti();
        lsprintf(str, "%X", i);
        draw_rectangle((unsigned char*)vram_addr, screen_x, back_color, 32, 16, 16, 16);
        put_string(vram_addr, screen_x, 32, 16, COLOR_FFFFFF, str);
      }
    }
  }
}

void wait_KBC_sendready(void){
  while(1){
    if((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0){
      break;
    }
  }
  return;
}

void init_keyboard(void){
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDATA, KBC_MODE);
  return;
}

void enable_mouse(void){
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDATA, MOUSECMD_ENABLE);
  return;
}





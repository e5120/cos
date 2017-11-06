#include "../include/graphic.h"

#define COLOR 16

void init_palette(void){
  static unsigned char rgb_table[3*COLOR] = {
    0x00, 0x00, 0x00,
    0xff, 0x00, 0x00,
    0x00, 0xff, 0x00,
    0xff, 0xff, 0x00,
    0x00, 0x00, 0xff,
    0xff, 0x00, 0xff,
    0x00, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xc6, 0xc6, 0xc6,
    0x84, 0x00, 0x00,
    0x00, 0x84, 0x00,
    0x84, 0x84, 0x00,
    0x00, 0x00, 0x84,
    0x84, 0x00, 0x84,
    0x00, 0x84, 0x84,
    0x84, 0x84, 0x84,
  };
  set_palette(COLOR, rgb_table);
  return;
}

void set_palette(int color, unsigned char* rgb){
  int i, eflags;
  eflags = io_load_eflags();
  io_cli();
  io_out8(0x03c8, 0);
  for(i = 0; i < color; ++i){
    io_out8(0x03c9, rgb[0] / 4);
    io_out8(0x03c9, rgb[1] / 4);
    io_out8(0x03c9, rgb[2] / 4);
    rgb += 3;   // アドレスを3進める
  }
  io_store_eflags(eflags);
  return;
}

/**
VRAMについて
現状、320(width)x200(height)の画面サイズ
左上の座標(0,0)、右下の座標(319,199)とすると、対応するVRAMのアドレスは、
0xa0000 + x + y * 320となる。
**/
void draw_rectangle(char* vram_addr, int screen_width, unsigned char color,
                    int lx, int ly, int width, int height){
  int x,y;
  int index;
  for(y = ly; y < ly+height; ++y){
    for(x = lx; x < lx+width; ++x){
      index = x + y * 320;
      vram_addr[index] = color;
    }
  }
  return;
}

void put_font(char* vram_addr, int screen_x, int x, int y, char color, char* font){
  int i;
  char* p;
  char d;
  for(i = 0; i < 16; ++i){
    p = vram_addr + (y + i) * screen_x + x;
    d = font[i];
    if((d & 0x80) != 0) p[0] = color;
    if((d & 0x40) != 0) p[1] = color;
    if((d & 0x20) != 0) p[2] = color;
    if((d & 0x10) != 0) p[3] = color;
    if((d & 0x08) != 0) p[4] = color;
    if((d & 0x04) != 0) p[5] = color;
    if((d & 0x02) != 0) p[6] = color;
    if((d & 0x01) != 0) p[7] = color;
  }
  return;
}

void put_string(char* vram_addr, int screen_x, int x, int y, char color, char* str){
  extern char hankaku[4096];
  int i;
  for(i = 0; str[i] != '\0'; ++i){
    put_font(vram_addr, screen_x, x + i * 8, y, color, hankaku + str[i] * 16);
  }
  return;
}


void init_desktop(char* vram_addr, int screen_x, int screen_y, unsigned char color){
  draw_rectangle(vram_addr, screen_x, color, 0, 0, screen_x, screen_y);

  // 四角形の描画
  // 当然、描画順を変えればVRAMの各要素の最終的な値が変わるため、描画結果も変わる
  draw_rectangle(vram_addr, screen_x, COLOR_C6C6C6, 0, screen_y - 20, screen_x, screen_y - 20);
  draw_rectangle(vram_addr, screen_x, COLOR_FFFFFF, 0, screen_y - 19, screen_x, screen_y - 19);
  draw_rectangle(vram_addr, screen_x, COLOR_C6C6C6, 0, screen_y - 18, screen_x, 18);

  draw_rectangle(vram_addr, screen_x, COLOR_FFFFFF, 3,  screen_y - 16, 56,  1);
  draw_rectangle(vram_addr, screen_x, COLOR_FFFFFF, 2,  screen_y - 16,  1, 15);
  draw_rectangle(vram_addr, screen_x, COLOR_848484, 3,  screen_y -  3, 56,  1);
  draw_rectangle(vram_addr, screen_x, COLOR_848484, 59, screen_y - 15,  1, 14);
  draw_rectangle(vram_addr, screen_x, COLOR_000000, 2,  screen_y -  2, 57,  1);
  draw_rectangle(vram_addr, screen_x, COLOR_000000, 60, screen_y - 16,  1, 15);

  draw_rectangle(vram_addr, screen_x, COLOR_848484, screen_x - 47, screen_y - 16,  43,  1);
  draw_rectangle(vram_addr, screen_x, COLOR_848484, screen_x - 47, screen_y - 15,   1, 15);
  draw_rectangle(vram_addr, screen_x, COLOR_FFFFFF, screen_x - 47, screen_y -  2,  44,  1);
  draw_rectangle(vram_addr, screen_x, COLOR_FFFFFF, screen_x -  3, screen_y - 16,   1, 16);

  // draw_rectangle((unsigned char*)vram_addr, screen_x, COLOR_FF0000, 20, 20, 100, 100);
  // draw_rectangle((unsigned char*)vram_addr, screen_x, COLOR_00FF00, 70, 50, 100, 100);
  // draw_rectangle((unsigned char*)vram_addr, screen_x, COLOR_0000FF, 120, 80, 100, 100);
  return;
}

void init_mouse_cursor(char *mouse, char bc){
  static char cursor[16][16] = {
    "**************..",
    "*ooooooooooo*...",
    "*oooooooooo*....",
    "*ooooooooo*.....",
    "*oooooooo*......",
    "*ooooooo*.......",
    "*ooooooo*.......",
    "*oooooooo*......",
    "*oooo*oooo*.....",
    "*ooo*..*ooo*....",
    "*oo*....*ooo*...",
    "*o*......*ooo*..",
    "**........*ooo*.",
    "*..........*ooo*",
    "............*oo*",
    ".............***",
  };
  int x, y;
  for (y = 0; y < 16; ++y){
    for (x = 0; x < 16; ++x){
      if(cursor[y][x] == '*'){
        mouse[y*16 + x] = COLOR_000000;
      }
      else if(cursor[y][x] == 'o'){
        mouse[y*16 + x] = COLOR_FFFFFF;
      }
      else if(cursor[y][x] == '.'){
        mouse[y*16 + x] = bc;
      }
    }
  }
  return;
}

void put_block(char* vram, int screen_x, int width, int height,
              int x, int y, char* buf, int width_per_line){
  int i, j;
  int vram_i, buf_i;
  for(i = 0; i < height; ++i){
    for(j = 0; j < width; ++j){
      vram_i = (y + i) * screen_x + (x + j);
      buf_i = i*width_per_line+j;
      vram[vram_i] = buf[buf_i];
    }
  }
  return;
}

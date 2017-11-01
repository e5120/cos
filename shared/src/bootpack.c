// 色の設定ファイルをインクルード
#include "color_set.h"

// 定数宣言
#define COLOR 16
#define SCREEN_X 320
#define SCREEN_Y 200

// asumfunc.asm内の関数
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

// プロトタイプ宣言
void init_palette(void);
void set_palette(int color, unsigned char *rgb);
void set_background(unsigned char* vram_addr, int screen_x, int screen_y, unsigned char color);
void draw_rectangle(unsigned char* vram_addr, int screen_width, unsigned char color,
                    int lx, int ly, int width, int height);

void HariMain(void){
  char* p;

  init_palette();

  p = (char*)0xa0000;    // VRAMの先頭アドレス
  // 背景色設定
  set_background((unsigned char*)p, SCREEN_X, SCREEN_Y, COLOR_008484);

  // 四角形の描画
  // 当然、描画順を変えればVRAMの各要素の最終的な値が変わるため、描画結果も変わる
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_C6C6C6, 0, SCREEN_Y - 20, SCREEN_X, SCREEN_Y - 20);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FFFFFF, 0, SCREEN_Y - 19, SCREEN_X, SCREEN_Y - 19);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_C6C6C6, 0, SCREEN_Y - 18, SCREEN_X, 18);

  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FFFFFF, 3,  SCREEN_Y - 16, 56,  1);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FFFFFF, 2,  SCREEN_Y - 16,  1, 15);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_848484, 3,  SCREEN_Y -  3, 56,  1);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_848484, 59, SCREEN_Y - 15,  1, 14);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_000000, 2,  SCREEN_Y -  2, 57,  1);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_000000, 60, SCREEN_Y - 16,  1, 15);

  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_848484, SCREEN_X - 47, SCREEN_Y - 16,  43,  1);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_848484, SCREEN_X - 47, SCREEN_Y - 15,   1, 15);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FFFFFF, SCREEN_X - 47, SCREEN_Y -  2,  44,  1);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FFFFFF, SCREEN_X -  3, SCREEN_Y - 16,   1, 16);


  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_FF0000, 20, 20, 100, 100);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_00FF00, 70, 50, 100, 100);
  draw_rectangle((unsigned char*)p, SCREEN_X, COLOR_0000FF, 120, 80, 100, 100);


  while(1){
    io_hlt();
  }
}

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
void draw_rectangle(unsigned char* vram_addr, int screen_width, unsigned char color,
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

void set_background(unsigned char* vram_addr, int screen_x, int screen_y, unsigned char color){
  draw_rectangle(vram_addr, screen_x, color, 0, 0, screen_x, screen_y);
}


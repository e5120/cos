#include "../include/window.h"

void make_window(unsigned char* buf, int xsize, int ysize, char* title, char act){
  char tc, tbc;
  if(act){
    tc = COLOR_FFFFFF;
    tbc = COLOR_000084;
  }
  else{
    tc = COLOR_C6C6C6;
    tbc = COLOR_848484;
  }
  draw_rectangle(buf, xsize, COLOR_C6C6C6, 0,         0,         xsize,     1);
  draw_rectangle(buf, xsize, COLOR_FFFFFF, 1,         1,         xsize - 2, 1);
  draw_rectangle(buf, xsize, COLOR_C6C6C6, 0,         0,         1,         ysize);
  draw_rectangle(buf, xsize, COLOR_FFFFFF, 1,         1,         1,         ysize - 2);
  draw_rectangle(buf, xsize, COLOR_848484, xsize - 2, 1,         1,         ysize - 2);
  draw_rectangle(buf, xsize, COLOR_000000, xsize - 1, 0,         1,         ysize);
  draw_rectangle(buf, xsize, COLOR_C6C6C6, 2,         2,         xsize - 4, ysize - 4);
  draw_rectangle(buf, xsize, tbc, 3,         3,         xsize - 6, 18);
  draw_rectangle(buf, xsize, COLOR_848484, 1,         ysize - 2, xsize - 2, 1);
  draw_rectangle(buf, xsize, COLOR_000000, 0,         ysize - 1, xsize,     1);

  put_string(buf, xsize, 24, 4, tc, title);

  static char close_btn[14][16] = {
    "ooooooooooooooo@",
    "oqqqqqqqqqqqqq$@",
    "oqqqqqqqqqqqqq$@",
    "oqqq@@qqqq@@qq$@",
    "oqqqq@@qq@@qqq$@",
    "oqqqqq@@@@qqqq$@",
    "oqqqqqq@@qqqqq$@",
    "oqqqqq@@@@qqqq$@",
    "oqqqq@@qq@@qqq$@",
    "oqqq@@qqqq@@qq$@",
    "oqqqqqqqqqqqqq$@",
    "oqqqqqqqqqqqqq$@",
    "o$$$$$$$$$$$$$$@",
    "@@@@@@@@@@@@@@@@"
  };

  int x, y;
  char c;
  for(y = 0; y < 14; ++y){
    for(x = 0; x < 16; ++x){
      c = close_btn[y][x];
      if(c == '@'){
        c = COLOR_000000;
      }
      else if(c == '$'){
        c = COLOR_848484;
      }
      else if(c == 'q'){
        c = COLOR_C6C6C6;
      }
      else{
        c = COLOR_FFFFFF;
      }
      buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
    }
  }
  return;
}

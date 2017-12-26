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
  draw_rectangle(buf, xsize, COLOR_848484, 1,         ysize - 2, xsize - 2, 1);
  draw_rectangle(buf, xsize, COLOR_000000, 0,         ysize - 1, xsize,     1);

  make_wtitle(buf, xsize, title, act);
  return;
}

void make_wtitle(unsigned char* buf, int xsize, char *title, char act){
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
  char c, tc, tbc;
  if(act != 0){
    tc = COLOR_FFFFFF;
    tbc = COLOR_000084;
  }
  else{
    tc = COLOR_C6C6C6;
    tbc = COLOR_848484;
  }
  draw_rectangle(buf, xsize, tbc, 3, 3, xsize - 6, 18);
  put_string(buf, xsize, 24, 4, tc, title);

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
}

void make_textbox8(LAYER *layer, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
  ++sx;
  ++sy;
	draw_rectangle(layer->buf, layer->bxsize, COLOR_848484, x0 - 2, y0 - 3, sx, 1);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_848484, x0 - 3, y0 - 3, 1, sy);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_FFFFFF, x0 - 3, y1 + 2, sx, 1);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_FFFFFF, x1 + 2, y0 - 3, 1, sy);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_000000, x0 - 1, y0 - 2, sx, 1);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_000000, x0 - 2, y0 - 2, 1, sy);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_C6C6C6, x0 - 2, y1 + 1, sx, 1);
	draw_rectangle(layer->buf, layer->bxsize, COLOR_C6C6C6, x1 + 1, y0 - 2, 1, sy);
	draw_rectangle(layer->buf, layer->bxsize, c,            x0 - 1, y0 - 1, sx, sy);
	return;
}

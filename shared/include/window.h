#pragma once
#include "define.h"
#include "graphic.h"

void make_window(unsigned char* buf, int xsize, int ysize, char* title, char act);
void make_wtitle(unsigned char* buf, int xsize, char *title, char act);
void make_textbox8(LAYER *sht, int x0, int y0, int sx, int sy, int c);

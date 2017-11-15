#pragma once
#include "memory.h"
#include "layer_s.h"

#define LAYER_USED  1

LAYER_CTL* layer_control_init(MEM_MAN* memman, unsigned char *vram, int xsize, int ysize);
LAYER* layer_alloc(LAYER_CTL* ctl);
void layer_setbuf(LAYER* layer, unsigned char* buf, int xsize, int ysize, int col_inv);
void layer_updown(LAYER* layer, int height);
void layer_refresh(LAYER* layer, int bx0, int by0, int bx1, int by1);
void layer_refreshsub(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1);
void layer_refreshmap(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0);
void layer_slide(LAYER* layer, int vx, int vy);
void layer_free(LAYER* layer);


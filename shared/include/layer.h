#pragma once
#include "memory.h"
#define MAX_LAYERS  256
#define LAYER_USED  1

typedef struct LAYER_SHEET{
    unsigned char *buf;
    int bxsize;
    int bysize;
    int vx;
    int vy;
    int color_inv;
    int height;
    int flags;
}LAYER;

typedef struct LAYER_CONTROL{
    unsigned char *vram;
    int xsize;
    int ysize;
    int top;
    LAYER* sheets[MAX_LAYERS];
    LAYER sheets0[MAX_LAYERS];
}LAYER_CTL;

LAYER_CTL* layer_control_init(MEM_MAN* memman, unsigned char *vram, int xsize, int ysize);
LAYER* layer_alloc(LAYER_CTL* ctl);
void layer_setbuf(LAYER* layer, unsigned char* buf, int xsize, int ysize, int col_inv);
void layer_updown(LAYER_CTL* ctl, LAYER* layer, int height);
void layer_refresh(LAYER_CTL* ctl, LAYER* layer, int bx0, int by0, int bx1, int by1);
void layer_refreshsub(LAYER_CTL* ctl, int vx0, int vy0, int vx1, int vy1);
void layer_slide(LAYER_CTL* ctl, LAYER* layer, int vx, int vy);
void layer_free(LAYER_CTL* ctl, LAYER* layer);


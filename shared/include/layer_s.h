#pragma once

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
    struct LAYER_CONTROL* ctl;
}LAYER;

typedef struct LAYER_CONTROL{
    unsigned char *vram, *map;
    int xsize;
    int ysize;
    int top;
    LAYER* sheets[MAX_LAYERS];
    LAYER sheets0[MAX_LAYERS];
}LAYER_CTL;

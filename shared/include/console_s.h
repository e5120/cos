#pragma once
#include "layer.h"

typedef struct CONSOLE{
    LAYER *layer;
    int cur_x;
    int cur_y;
    int cur_c;
}CONSOLE;
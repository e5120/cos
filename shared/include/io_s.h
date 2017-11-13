#pragma once
typedef struct MOUSE_DECODE{
    unsigned char buf[3];
    unsigned char phase;
    int x;
    int y;
    int btn;
}MOUSE_DEC;

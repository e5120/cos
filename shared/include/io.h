#pragma once
#include "asmfunc_def.h"

#define PORT_KEYDATA            0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

typedef struct MOUSE_DECODE{
    unsigned char buf[3];
    unsigned char phase;
    int x;
    int y;
    int btn;
}MOUSE_DEC;

void wait_KBC_sendready(void);
void init_keyboard(void);
void enable_mouse(MOUSE_DEC* mdec);
int mouse_decode(MOUSE_DEC* mdec, unsigned char data);

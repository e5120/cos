#pragma once
#include "asmfunc_def.h"
#include "fifo.h"
#include "interrupt.h"

#define PORT_KEYDATA            0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4
#define PORT_KEYDATA  0x0060

typedef struct MOUSE_DECODE{
    unsigned char buf[3];
    unsigned char phase;
    int x;
    int y;
    int btn;
}MOUSE_DEC;

FIFO32 *keyfifo, *mousefifo;
int keydata, mousedata;

void wait_KBC_sendready(void);
void init_keyboard(FIFO32* fifo, int data);
void interrupt_handler21(int *esp);

void enable_mouse(FIFO32* fifo, int data, MOUSE_DEC* mdec);
void interrupt_handler27(int *esp);
int mouse_decode(MOUSE_DEC* mdec, unsigned char data);

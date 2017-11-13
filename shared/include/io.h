#pragma once
#include "io_s.h"
#include "fifo.h"

#define PORT_KEYDATA            0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4
#define PORT_KEYDATA  0x0060



void wait_KBC_sendready(void);
void init_keyboard(FIFO32* fifo, int data);
void interrupt_handler21(int *esp);

void enable_mouse(FIFO32* fifo, int data, MOUSE_DEC* mdec);
void interrupt_handler27(int *esp);
int mouse_decode(MOUSE_DEC* mdec, unsigned char data);


static char keytable[0x54] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0, 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0, 0, ']',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6',
    '+', '1', '2', '3', '0', '_'
};

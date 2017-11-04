#pragma once
#include "graphic.h"
#include "macro.h"
#include "dsctbl.h"
#include "define.h"
#include "address.h"
#include "interrupt.h"
#include "fifo.h"

#define PORT_KEYDATA            0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

// asmhead.asmのBOOT_INFO参照
typedef struct BOOT_INFOMATION{
  char  cyls;       // 0x0ff0 (1byte)
  char  leds;       // 0x0ff1 (1byte)
  char  vmode;      // 0x0ff2 (1byte)
  char  reserve;    // 0x0ff3 (使われてない)
  short screen_x;   // 0x0ff4 (2byte = short型)
  short screen_y;   // 0x0ff6 (2byte = short型)
  char* vram;       // 0x0ff8 (1byte)
}BOOT_INFO;

void wait_KBC_sendready(void);
void init_keyboard(void);
void enable_mouse(void);

// asumfunc.asm内の関数
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(short, int);
void load_idtr(short, int);
void asm_interrupt_handler21(void);
void asm_interrupt_handler27(void);
void asm_interrupt_handler2c(void);
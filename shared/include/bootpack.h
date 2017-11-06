#pragma once
#include "graphic.h"
#include "macro.h"
#include "dsctbl.h"
#include "define.h"
#include "address.h"
#include "fifo.h"
#include "io.h"
#include "memory.h"
#include "asmfunc_def.h"

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


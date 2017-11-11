#pragma once
#include "address.h"
#include "interrupt.h"
#include "asmfunc_def.h"

// GDTの情報
typedef struct SEGMENT_DESCRIPTION{
  short limit_low;
  short base_low;
  char base_mid;
  char access_right;
  char limit_high;
  char base_high;
}SEG_DESC;

// IDTの情報
typedef struct GATE_DESCRIPTION{
  short offset_low;
  short selector;
  char dw_count;
  char access_right;
  short offset_high;
}GATE_DESC;

// Task Status Segment
typedef struct TASK_STATUS_SEGMENT_32{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
}TSS32;

// プロトタイプ宣言
void init_gdtidt(void);
void set_segmdesc(SEG_DESC* sd, unsigned int limit, int base, int ar);
void set_gatedesc(GATE_DESC* gd, int offset, int selector, int ar);

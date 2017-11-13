#pragma once
#include "asmfunc_def.h"
#include "address.h"
#include "dsctbl_s.h"

// プロトタイプ宣言
void init_gdtidt(void);
void set_segmdesc(SEG_DESC* sd, unsigned int limit, int base, int ar);
void set_gatedesc(GATE_DESC* gd, int offset, int selector, int ar);

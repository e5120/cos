#pragma once
#include "timer.h"

TIMER* mt_timer;
int mt_tr;

void mt_init(void);
void mt_taskswitch(void);

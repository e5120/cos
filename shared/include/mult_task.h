#pragma once
#include "memory.h"
#include "dsctbl.h"
#include "mult_task_s.h"
#include "timer.h"

TASK* task_init(MEM_MAN* memman);
TASK* task_alloc(void);
void task_run(TASK* task);
void task_switch(void);
void task_sleep(TASK* task);
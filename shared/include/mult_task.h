#pragma once
#include "memory.h"
#include "dsctbl.h"
#include "mult_task_s.h"
#include "timer.h"

#define TASK_GDT0   3

TASK* task_init(MEM_MAN* memman);
TASK* task_alloc(void);
void task_run(TASK* task, int priority);
void task_switch(void);
void task_sleep(TASK* task);
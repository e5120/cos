#pragma once
#define MAX_TASKS   1000
#define TASK_GDT0   3

// Task Status Segment
typedef struct TASK_STATUS_SEGMENT_32{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
}TSS32;

typedef struct TASK{
    int sel;
    int flags;
    TSS32 tss;
}TASK;

typedef struct TASK_CONTROL{
    int running;
    int now;
    TASK* tasks[MAX_TASKS];
    TASK tasks0[MAX_TASKS];
}TASK_CTL;

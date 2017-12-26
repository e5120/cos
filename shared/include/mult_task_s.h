#pragma once
#define MAX_TASKS   1000
#define MAX_TASKS_LV 100
#define MAX_TASKLEVELS 10

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
    int level;
    int priority;
    TSS32 tss;
}TASK;

typedef struct TASKLEVEL{
    int running;        // 動作しているタスク数
    int now;            // 現在動作しているタスクがどれか分かるための変数
    TASK *tasks[MAX_TASKS_LV];
}TASK_LV;

typedef struct TASK_CONTROL{
    int now_lv;         // 現在動作中の
    char lv_change;     // 次回タスクスイッチ時にレベルを変えるかどうか
    TASK_LV level[MAX_TASKLEVELS];
    TASK tasks0[MAX_TASKS];
}TASK_CTL;

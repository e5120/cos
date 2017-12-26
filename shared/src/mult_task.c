#include "../include/mult_task.h"

TASK_CTL* taskctl;
TIMER*  task_timer;

TASK* task_init(MEM_MAN* memman){
  int i;
  TASK *task, *idle;
  SEG_DESC* gdt = (SEG_DESC*)ADDR_GDT;
  taskctl = (TASK_CTL*)memory_manage_alloc_4k(memman, sizeof(TASK_CTL));
  for(i = 0;i < MAX_TASKS; ++i){
    taskctl->tasks0[i].flags = 0;
    taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
    set_segmdesc(gdt + TASK_GDT0 + i, 103, (int)&taskctl->tasks0[i].tss, AR_TSS32);
  }
  for(i = 0; i < MAX_TASKLEVELS; ++i){
    taskctl->level[i].running = 0;
    taskctl->level[i].now = 0;
  }
  task = task_alloc();
  task->flags = 2;
  task->priority = 2;
  task->level = 0;
  task_add(task);
  task_switchsub();
  load_tr(task->sel);
  task_timer = timer_alloc();
  settimer(task_timer, task->priority);

  idle = task_alloc();
  idle->tss.esp = memory_manage_alloc_4k(memman, 64 * 1024) + 64 * 1024;
  idle->tss.eip = (int)&task_idle;
  idle->tss.es = 1 * 8;
  idle->tss.cs = 2 * 8;
  idle->tss.ss = 1 * 8;
  idle->tss.ds = 1 * 8;
  idle->tss.fs = 1 * 8;
  idle->tss.gs = 1 * 8;
  task_run(idle, MAX_TASKLEVELS - 1, 1);

  return task;
}

TASK* task_alloc(void){
  int i;
  TASK* task;
  for(i = 0; i < MAX_TASKS; ++i){
    if(taskctl->tasks0[i].flags == 0){
      task = &taskctl->tasks0[i];
      task->flags = 1;
      task->tss.eflags = 0x00000202;
      task->tss.eax = 0;
      task->tss.ecx = 0;
      task->tss.edx = 0;
      task->tss.ebx = 0;
      task->tss.ebp = 0;
      task->tss.esi = 0;
      task->tss.edi = 0;
      task->tss.es = 0;
      task->tss.ds = 0;
      task->tss.fs = 0;
      task->tss.gs = 0;
      task->tss.ldtr = 0;
      task->tss.iomap = 0x40000000;
      return task;
    }
  }
  return 0;
}

void task_run(TASK* task, int level, int priority){
  if(level < 0){
    level = task->level;  // レベルを変更しない
  }
  if(priority > 0){
    task->priority = priority;
  }
  if(task->flags == 2 && task->level != level){ // 動作中のレベルの変更
    // この動作によりflagsが1となり、下のifも実行される
    task_remove(task);
  }
  if(task->flags != 2){
    // スリープから起こされる
    task->level = level;
    task_add(task);
  }
  taskctl->lv_change = 1;
  return;
}

void task_switch(void){
  TASK_LV *t1 = &taskctl->level[taskctl->now_lv];
  TASK *new_task, *now_task = t1->tasks[t1->now];

  ++t1->now;
  if(t1->now == t1->running){
    t1->now = 0;
  }
  if(taskctl->lv_change != 0){
    task_switchsub();
    t1 = &taskctl->level[taskctl->now_lv];
  }
  new_task = t1->tasks[t1->now];
  settimer(task_timer, new_task->priority);
  if(new_task != now_task){
    farjmp(0, new_task->sel);
  }
  return;
}

void task_sleep(TASK* task){
    TASK *now_task;
    if(task->flags == 2){
      now_task = task_now();
      task_remove(task);
      if(task == now_task){
        // 自分自身のスリープなので、タスクスイッチ
        task_switchsub();
        now_task = task_now();  // 設定後に現在のタスクを教えてもらう
        farjmp(0, now_task->sel);
      }
    }
    return;
}

TASK *task_now(void){
  TASK_LV *t1 = &taskctl->level[taskctl->now_lv];
  return t1->tasks[t1->now];
}

void task_add(TASK *task){
  TASK_LV *t1 = &taskctl->level[task->level];
  t1->tasks[t1->running] = task;
  ++t1->running;
  task->flags = 2;  // 動作中
  return;
}

void task_remove(TASK *task){
  int i;
  TASK_LV *t1 = &taskctl->level[task->level];

  for(i = 0; i < t1->running; ++i){
    if(t1->tasks[i] == task){
      break;
    }
  }
  --t1->running;
  if(i < t1->now){
    // iがnowの値未満の時nowもずれるので合わせる
    --t1->now;
  }
  if(t1->now >= t1->running){
    // nowの値がおかしければ修正
    t1->now = 0;
  }
  task->flags = 1;    // スリープ中

  // ずらす
  for(; i < t1->running; ++i){
    t1->tasks[i] = t1->tasks[i + 1];
  }
  return;
}

void task_switchsub(void){
  int i;
  for(i = 0; i < MAX_TASKLEVELS; ++i){
    if(taskctl->level[i].running > 0){
      break;
    }
  }
  taskctl->now_lv = i;
  taskctl->lv_change = 0;
  return;
}

void task_idle(void){
  while(1){
    io_hlt();
  }
}


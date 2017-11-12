#include "../include/timer.h"
#include "../include/mult_task.h"

void init_pit(void){
  int i;
  TIMER* t;
  io_out8(PIT_CTRL, 0x34);
  // 割り込み周期の上位8bitと下位8bitを分ける（今回は100Hz）
  io_out8(PIT_CNT, 0x9c);
  io_out8(PIT_CNT, 0x2e);
  timer_ctl.count = 0;
  for(i = 0; i < MAX_TIMER; ++i){
    timer_ctl.timer0[i].flags = 0;
  }
  t = timer_alloc();
  t->timeout = 0xfffffff;
  t->flags = TIMER_USED;
  t->next = 0;
  timer_ctl.timer = t;
  timer_ctl.next = 0xfffffff;
  return;
}

void interrupt_handler20(int *esp){
  TIMER* timer;
  char ts = 0;

  io_out8(PIC0_OCW2, 0x60);
  ++timer_ctl.count;
  if(timer_ctl.next > timer_ctl.count){
    return;
  }
  timer = timer_ctl.timer;
  while(1){
    if(timer->timeout > timer_ctl.count){
      break;
    }
    timer->flags = TIMER_ALLOC;
    if(timer != mt_timer){
      put_fifo32(timer->fifo, timer->data);
    }
    else{
      ts = 1;
    }
    timer = timer->next;
  }

  timer_ctl.timer = timer;
  timer_ctl.next = timer_ctl.timer->timeout;
  if(ts != 0){
    mt_taskswitch();
  }
  return;
}

void init_timer(TIMER* timer, FIFO32* fifo, int data){
  timer->fifo = fifo;
  timer->data = data;
  return;
}

TIMER* timer_alloc(void){
  int i;
  for(i = 0; i < MAX_TIMER; ++i){
    if(timer_ctl.timer0[i].flags == TIMER_FREE){
      timer_ctl.timer0[i].flags = TIMER_ALLOC;
      return &timer_ctl.timer0[i];
    }
  }
  return 0;
}


void timer_free(TIMER* timer){
  timer->flags = TIMER_FREE;
  return;
}

// timer_ctl.countがオーバフローしたらおかしくなるので、要改善
void settimer(TIMER* timer, unsigned int timeout){
  int e;
  TIMER *t, *s;
  timer->timeout = timeout + timer_ctl.count;
  timer->flags = TIMER_USED;
  e = io_load_eflags();
  io_cli();

  t  =timer_ctl.timer;
  if(timer->timeout <= t->timeout){
    timer_ctl.timer = timer;
    timer->next = t;
    timer_ctl.next  = timer->timeout;
    io_store_eflags(e);
    return;
  }

  while(1){
    s = t;
    t = t->next;
    if(timer->timeout <= t->timeout){
      s->next = timer;
      timer->next = t;
      io_store_eflags(e);
      return;
    }
  }
}


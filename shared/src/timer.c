#include "../include/timer.h"

TIMER_CTL timer_ctl;

void init_pit(void){
  int i;
  io_out8(PIT_CTRL, 0x34);
  // 割り込み周期の上位8bitと下位8bitを分ける（今回は100Hz）
  io_out8(PIT_CNT, 0x9c);
  io_out8(PIT_CNT, 0x2e);
  timer_ctl.count = 0;
  timer_ctl.next = 0xfffffff;
  timer_ctl.used = 0;
  for(i = 0; i < MAX_TIMER; ++i){
    timer_ctl.timer0[i].flags = 0;
  }
  return;
}

void interrupt_handler20(int *esp){
  int i, j;
  io_out8(PIC0_OCW2, 0x60);
  ++timer_ctl.count;
  if(timer_ctl.next > timer_ctl.count){
    return;
  }
  for(i = 0; i < timer_ctl.used; ++i){
    if(timer_ctl.timer[i]->timeout > timer_ctl.count){
      break;
    }
    timer_ctl.timer[i]->flags = TIMER_ALLOC;
    put_fifo(timer_ctl.timer[i]->fifo, timer_ctl.timer[i]->data);
  }
  for(j = 0; j < timer_ctl.used; ++j){
    timer_ctl.timer[j] = timer_ctl.timer[i + j];
  }
  if(timer_ctl.used > 0){
    timer_ctl.next = timer_ctl.timer[0]->timeout;
  }
  else{
    timer_ctl.next = 0xfffffff;
  }
  return;
}

void init_timer(TIMER* timer, FIFO* fifo, unsigned char data){
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
  int e, i, j;
  timer->timeout = timeout + timer_ctl.count;
  timer->flags = TIMER_USED;
  e = io_load_eflags();
  io_cli();
  for(i = 0; i< timer_ctl.used; ++i){
    if(timer_ctl.timer[i]->timeout >= timer->timeout){
      break;
    }
  }
  for(j = timer_ctl.used; j > i; --j){
    timer_ctl.timer[j] = timer_ctl.timer[j - 1];
  }
  ++timer_ctl.used;
  timer_ctl.timer[i] = timer;
  timer_ctl.next = timer_ctl.timer[0]->timeout;
  io_store_eflags(e);
  return;
}


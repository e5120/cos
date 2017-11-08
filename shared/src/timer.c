#include "../include/timer.h"

TIMER_CTL timer_ctl;

void init_pit(void){
  io_out8(PIT_CTRL, 0x34);
  // 割り込み周期の上位8bitと下位8bitを分ける（今回は100Hz）
  io_out8(PIT_CNT, 0x9c);
  io_out8(PIT_CNT, 0x2e);
  timer_ctl.count = 0;
  timer_ctl.timeout = 0;
  return;
}

void interrupt_handler20(int *esp){
  io_out8(PIC0_OCW2, 0x60);
  ++timer_ctl.count;
  if(timer_ctl.timeout > 0){
    --timer_ctl.timeout;
    if(timer_ctl.timeout == 0){
      put_fifo(timer_ctl.fifo, timer_ctl.data);
    }
  }
  return;
}

void settimer(unsigned int timeout, FIFO* fifo, unsigned char data){
  int eflags;
  eflags = io_load_eflags();
  io_cli();
  timer_ctl.timeout = timeout;
  timer_ctl.fifo = fifo;
  timer_ctl.data = data;
  io_store_eflags(eflags);
  return;
}


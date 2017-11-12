#include "../include/mult_task.h"

void mt_init(void){
  mt_timer = timer_alloc();
  settimer(mt_timer, 2);
  mt_tr = 3 * 8;
  return;
}


void mt_taskswitch(void){
  if(mt_tr == 3 * 8){
    mt_tr = 4 * 8;
  }
  else{
    mt_tr = 3 * 8;
  }
  settimer(mt_timer, 2);
  farjmp(0, mt_tr);
  return;
}
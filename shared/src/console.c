#include "../include/console.h"

void console_task(LAYER *layer){
  FIFO32 fifo;
  TIMER *timer;
  TASK *task = task_now();
  int i, fifobuf[FIFO_MAX_BUF], cursor_x = 8, cursor_c = COLOR_000000;
  init_fifo32(&fifo, FIFO_MAX_BUF, fifobuf, task);

  timer = timer_alloc();
  init_timer(timer, &fifo, 1);
  settimer(timer, 50);

  while(1){
    io_cli();
    if(fifo_status32(&fifo) == 0){
      task_sleep(task);
      io_sti();
    }
    else{
      i = get_fifo32(&fifo);
      io_sti();
      if(i <= 1){
        if(i != 0){
          init_timer(timer, &fifo, 0);
          cursor_c = COLOR_FFFFFF;

        }
        else{
          init_timer(timer, &fifo, 1);
          cursor_c = COLOR_000000;
        }
      }
      settimer(timer, 50);
      draw_rectangle((char*)layer->buf, layer->bxsize, cursor_c, cursor_x, 28, 8, 44);
      layer_refresh(layer, cursor_x, 28, cursor_x + 8, 44);
    }
  }
}
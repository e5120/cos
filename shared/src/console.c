#include "../include/console.h"

void console_task(LAYER *layer){
  TIMER *timer;
  TASK *task = task_now();
  char s[2];
  int i, fifobuf[FIFO_MAX_BUF], cursor_x = 16, cursor_c = COLOR_000000;

  init_fifo32(&task->fifo, FIFO_MAX_BUF, fifobuf, task);
  timer = timer_alloc();
  init_timer(timer, &task->fifo, 1);
  settimer(timer, 50);

  // プロンプト再表示
  put_string_layer(layer, 8, 28, COLOR_FFFFFF, COLOR_000000, ">", 1);

  while(1){
    io_cli();
    if(fifo_status32(&task->fifo) == 0){
      task_sleep(task);
      io_sti();
    }
    else{
      i = get_fifo32(&task->fifo);
      io_sti();
      if(i <= 1){
        if(i != 0){
          init_timer(timer, &task->fifo, 0);
          cursor_c = COLOR_FFFFFF;
        }
        else{
          init_timer(timer, &task->fifo, 1);
          cursor_c = COLOR_000000;
        }
        settimer(timer, 50);
      }
      if(KEY_BOTTOM <= i && i < KEY_TOP){
        if(i == 8 + KEY_BOTTOM){    // backspace
          if(cursor_x > 16){
            put_string_layer(layer, cursor_x, 28, COLOR_FFFFFF, COLOR_000000, " ", 1);
            cursor_x -= 8;
          }
        }
        else{   // 一般文字
          if(cursor_x < 240){
            s[0] = i - KEY_BOTTOM;
            s[1] = '\0';
            put_string_layer(layer, cursor_x, 28, COLOR_FFFFFF, COLOR_000000, s, 1);
            cursor_x += 8;
          }
        }
      }
      draw_rectangle((char*)layer->buf, layer->bxsize, cursor_c, cursor_x, 28, 8, 44);
      layer_refresh(layer, cursor_x, 28, cursor_x + 8, 44);
    }
  }
}
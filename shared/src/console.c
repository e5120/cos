#include "../include/console.h"

void console_task(LAYER *layer, unsigned int memtotal){
  TIMER *timer;
  TASK *task = task_now();
  char s[STR_MAX_BUF], cmdline[STR_MAX_BUF];
  MEM_MAN *memman = (MEM_MAN*)MEMMANAGER_ADDR;
  FILE_INFO *finfo = (FILE_INFO*)(ADDR_DISKIMG + 0x002600);
  int i, fifobuf[FIFO_MAX_BUF], cursor_x = 16, cursor_y = 28, cursor_c = -1;
  int x, y;

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
          if(cursor_c >= 0){
            cursor_c = COLOR_FFFFFF;
          }
        }
        else{
          init_timer(timer, &task->fifo, 1);
          if(cursor_c >= 0){
            cursor_c = COLOR_000000;
          }
        }
        settimer(timer, 50);
      }
      if(i == 2){
        cursor_c = COLOR_FFFFFF;
      }
      if(i == 3){
        draw_rectangle((char*)layer->buf, layer->bxsize, COLOR_000000, cursor_x, cursor_y, 8, 16);
        cursor_c = -1;
      }
      if(KEY_BOTTOM <= i && i < KEY_TOP){
        if(i == 8 + KEY_BOTTOM){    // backspace
          if(cursor_x > 16){
            put_string_layer(layer, cursor_x, cursor_y, COLOR_FFFFFF, COLOR_000000, " ", 1);
            cursor_x -= 8;
          }
        }
        else if(i == 10 + KEY_BOTTOM){    // enter
          put_string_layer(layer, cursor_x, cursor_y, COLOR_FFFFFF, COLOR_000000, " ", 1);
          cmdline[cursor_x / 8 - 2] = 0;
          cursor_y = cons_newline(cursor_y, layer);
          // コマンド実行
            if(lstrcmp(cmdline, "mem")){
              // memコマンド
              lsprintf(s, "total %dMB", memtotal / (1024 * 1024));
              put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, s, get_length(s));
              cursor_y = cons_newline(cursor_y, layer);
              lsprintf(s, "free %dKB", memory_manage_total(memman) / 1024);
              put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, s, get_length(s));
              cursor_y = cons_newline(cursor_y, layer);
              cursor_y = cons_newline(cursor_y, layer);
            }
            else if(lstrcmp(cmdline, "clear")){
              for(y = 28; y < 28 + 128; ++y){
                for(x = 8; x < 8 + 240; ++x){
                  layer->buf[x + y * layer->bxsize] = COLOR_000000;
                }
              }
              layer_refresh(layer, 8, 28, 8 + 240, 28 + 128);
              cursor_y = 28;
            }
            else if(lstrcmp(cmdline, "ls")){
              for(x = 0; x < 224; ++x){
                if(finfo[x].name[0] == 0x00){
                  break;
                }
                if(finfo[x].name[0] != 0xe5){
                  if((finfo[x].type & 0x18) == 0){
                    lsprintf(s, "filename.ext %d", finfo[x].size);
                    for(y = 0; y < 8; ++y){
                      s[y] = finfo[x].name[y];
                    }
                    s[9] = finfo[x].ext[0];
                    s[10] = finfo[x].ext[1];
                    s[11] = finfo[x].ext[2];
                    put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, s, get_length(s));
                    cursor_y = cons_newline(cursor_y, layer);
                  }
                }
              }
              cursor_y = cons_newline(cursor_y, layer);
            }
            else if(cmdline[0] != 0){
              put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, "Bad command.", 12);
              cursor_y = cons_newline(cursor_y, layer);
              cursor_y = cons_newline(cursor_y, layer);
            }
            put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, ">", 1);
            cursor_x = 16;
        }
        else{   // 一般文字
          if(cursor_x < 240){
            s[0] = i - KEY_BOTTOM;
            s[1] = '\0';
            cmdline[cursor_x / 8 - 2] = i - KEY_BOTTOM;
            put_string_layer(layer, cursor_x, cursor_y, COLOR_FFFFFF, COLOR_000000, s, 1);
            cursor_x += 8;
          }
        }
      }
      // カーソル再表示
      if(cursor_c >= 0){
        draw_rectangle((char*)layer->buf, layer->bxsize, cursor_c, cursor_x, cursor_y, 8, 16);
      }
      layer_refresh(layer, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
    }
  }
}

int cons_newline(int cursor_y, LAYER *layer){
  int x, y;
  if(cursor_y < 28 + 112){
    cursor_y += 16;
  }
  else{
    // スクロール
    for(y = 28; y < 28 + 112; ++y){
      for(x = 8; x < 8 + 240; ++x){
        layer->buf[x + y * layer->bxsize] = layer->buf[x + (y + 16) * layer->bxsize];
      }
    }
    for(y = 28 + 112; y < 28 + 128; ++y){
      for(x = 8; x < 8 + 240; ++x){
        layer->buf[x + y * layer->bxsize] = COLOR_000000;
      }
    }
    layer_refresh(layer, 8, 28, 8 + 240, 28 + 128);
  }
  return cursor_y;
}
#include "../include/console.h"

void console_task(LAYER *layer, unsigned int memtotal){
  TIMER *timer;
  TASK *task = task_now();
  char s[STR_MAX_BUF], cmdline[STR_MAX_BUF], *p;
  MEM_MAN *memman = (MEM_MAN*)MEMMANAGER_ADDR;
  FILE_INFO *finfo = (FILE_INFO*)(ADDR_DISKIMG + 0x002600);
  SEG_DESC *gdt = (SEG_DESC*)ADDR_GDT;
  int fifobuf[FIFO_MAX_BUF], cursor_x = 16, cursor_y = 28, cursor_c = -1;
  int i, x, y;
  int *fat = (int*)memory_manage_alloc_4k(memman, 4 * 2880);

  init_fifo32(&task->fifo, FIFO_MAX_BUF, fifobuf, task);
  timer = timer_alloc();
  init_timer(timer, &task->fifo, 1);
  settimer(timer, 50);

  file_readfat(fat, (unsigned char*)(ADDR_DISKIMG + 0x0002000));

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
            else if(lstrncmp(cmdline, "cat ", 4)){
              // catコマンド
              for(y = 0; y < 11; ++y){
                s[y] = ' ';
              }
              y = 0;
              for(x = 4; y < 11 && cmdline[x] != 0; ++x){
                if(cmdline[x] == '.' && y <= 8){
                  y = 8;
                }
                else{
                  s[y] = cmdline[x];
                  if('a' <= s[y] && s[y] <= 'z'){
                    s[y] -= 0x20;
                  }
                  ++y;
                }
              }
              // フィアル探索
              for(x = 0; x < 224;){
                if(finfo[x].name[0] == 0x00){
                  break;
                }
                if((finfo[x].type & 0x18) == 0){
                  for(y = 0; y < 11; ++y){
                    if(finfo[x].name[y] != s[y]){
                      goto next_file;
                    }
                  }
                  break;    // ファイル発見
                }
next_file:
                ++x;
              }
              if(x < 224 && finfo[x].name[0] != 0x00){
                // ファイル発見した場合
                y = finfo[x].size;
                p = (char*)memory_manage_alloc_4k(memman, finfo[x].size);
                file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char*)(ADDR_DISKIMG + 0x003e00));
                cursor_x = 8;
                for(y = 0; y < finfo[x].size; ++y){
                  s[0] = p[y];
                  s[1] = '\0';
                  if(s[0] == 0x09){   // tab
                    while(1){
                      put_string_layer(layer, cursor_x, cursor_y, COLOR_FFFFFF, COLOR_000000, " ", 1);
                      cursor_x += 8;
                      if(cursor_x == 8 + 240){
                        cursor_x = 8;
                        cursor_y = cons_newline(cursor_y, layer);
                      }
                      if(((cursor_x - 8) & 0x1f) == 0){
                        break;
                      }
                    }
                  }
                  else if(s[0] == 0x0a){    // 改行
                    cursor_x = 8;
                    cursor_y = cons_newline(cursor_y, layer);
                  }
                  else if(s[0] == 0x0d){    // 復帰
                    // 現状何もしない
                  }
                  else{
                    put_string_layer(layer, cursor_x, cursor_y, COLOR_FFFFFF, COLOR_000000, s, 1);
                    cursor_x += 8;
                    if(cursor_x == 8 + 240){
                      cursor_x = 8;
                      cursor_y = cons_newline(cursor_y, layer);
                    }
                  }
                }
                memory_manage_free_4k(memman, (int)p, finfo[x].size);
              }
              else{
                // ファイル発見できなかった場合
                put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, "File not found.", 15);
                cursor_y = cons_newline(cursor_y, layer);
              }
              cursor_y = cons_newline(cursor_y, layer);
            }
            else if(lstrcmp(cmdline, "hlt")){
              for(y = 0; y < 11; ++y){
                s[y] = ' ';
              }
              s[0] = 'H';
              s[1] = 'L';
              s[2] = 'T';
              s[8] = 'B';
              s[9] = 'I';
              s[10] = 'N';
              for(x = 0; x < 224; ){
                if(finfo[x].name[0] == 0x00){
                  break;
                }
                if((finfo[x].type & 0x18) == 0){
                  for(y = 0; y <11; ++y){
                    if(finfo[x].name[y] != s[y]){
                      goto halt_next_file;
                    }
                  }
                  break;
                }
halt_next_file:
                ++x;
              }
              if(x < 224 && finfo[x].name[0] != 0x00){
                // ファイル発見
                p = (char*) memory_manage_alloc_4k(memman, finfo[x].size);
                file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char*)(ADDR_DISKIMG + 0x003e00));
                set_segmdesc(gdt + 1003, finfo[x].size - 1, (int)p, AR_CODE32_ER);
                farjmp(0, 1003 * 8);
                memory_manage_free_4k(memman, (int)p, finfo[x].size);
              }
              else{
                put_string_layer(layer, 8, cursor_y, COLOR_FFFFFF, COLOR_000000, "File not found.", 15);
                cursor_y = cons_newline(cursor_y, layer);
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

void file_readfat(int *fat, unsigned char *img){
  int i, j = 0;
  for(i = 0; i <2880; i+= 2){
    fat[i + 0] = (img[j + 0] | img[j + 1] << 8) & 0xfff;
    fat[i + 1] = (img[j + 1] | img[j + 2] << 4) & 0xfff;
    j += 3;
  }
  return;
}

void file_loadfile(int clustno, int size, char *buf, int *fat, char *img){
  int i;
  while(1){
    if(size <= 512){
      for(i = 0; i < size; ++i){
        buf[i] = img[clustno * 512 + i];
      }
      break;
    }
    for(i = 0; i < 512; ++i){
      buf[i] = img[clustno * 512 + i];
    }
    size -= 512;
    buf += 512;
    clustno = fat[clustno];
  }
  return;
}
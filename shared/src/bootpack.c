#include "../include/bootpack.h"

void HariMain(void){
  // デバッグ用
  int key_to = 0;
  // boot関連 (asmhead.asmでのBOOT_INFO先頭番地)
  BOOT_INFO *binfo = (BOOT_INFO*)ADDR_BOOTINFO;
  // メモリ関連
  unsigned int memtotal;           // メモリ総量
  MEM_MAN *memman = (MEM_MAN*)MEMMANAGER_ADDR;  // メモリ管理用構造体
  // レイヤー関連
  char str[STR_MAX_BUF];           // 画面表示用の文字列格納
  int back_color, cursor_color, cursor_x;    // 背景色,カーソルの色,カーソルのx座標
  unsigned char *buf_back, buf_mouse[MOUSE_MAX_BUF], *buf_window, *buf_cons;
  LAYER_CTL *layer_ctl;
  LAYER *layer_back, *layer_mouse, *layer_window, *layer_cons;
  // I/O関連
  int mouse_x, mouse_y;            // マウスの(x,y)座標
  MOUSE_DEC mdec;                  // マウスデコードの構造体
  // 割り込み関連
  int i, fifobuf[FIFO_MAX_BUF];    // i：割り込み時の値保存
  FIFO32 fifo;
  // タスク関連
  TASK   *task_a, *task_cons;
  TIMER *timer;

  init_gdtidt();
  init_pic();
  io_sti();

  init_fifo32(&fifo, FIFO_MAX_BUF, fifobuf, 0);

  init_pit();
  // I/O初期化
  init_keyboard(&fifo, KEY_BOTTOM);
  enable_mouse(&fifo, MOUSE_BOTTOM, &mdec);
  io_out8(PIC0_IMR, 0xf8);    // PITとPIC1とキーボードを許可
  io_out8(PIC1_IMR, 0xef);    // マウスを許可
  // メモリチェック
  memtotal = memtest(0x00400000, 0xbfffffff);
  memory_manage_init(memman);
  memory_manage_free(memman, 0x00001000, 0x0009e000);
  memory_manage_free(memman, 0x00400000, memtotal - 0x00400000);

  init_palette();
  // 背景色設定
  back_color = COLOR_008484;
  // レイヤー設定
  layer_ctl = layer_control_init(memman, (unsigned char*)binfo->vram, binfo->screen_x, binfo->screen_y);
  task_a = task_init(memman);
  fifo.task = task_a;
  task_run(task_a, 1, 0);

  layer_back = layer_alloc(layer_ctl);
  buf_back = (unsigned char*)memory_manage_alloc_4k(memman, binfo->screen_x * binfo->screen_y);
  layer_setbuf(layer_back, buf_back, binfo->screen_x, binfo->screen_y, -1);
  init_desktop((char*)buf_back, binfo->screen_x, binfo->screen_y, back_color);

  layer_window = layer_alloc(layer_ctl);
  buf_window = (unsigned char*)memory_manage_alloc_4k(memman, 160 * 52);
  layer_setbuf(layer_window, buf_window, 144, 52, -1);
  make_window(buf_window, 144, 52, "task_a", 1);
  cursor_x = 8;
  cursor_color = COLOR_000000;

  timer = timer_alloc();
  init_timer(timer, &fifo, 1);
  settimer(timer, 50);

  layer_mouse = layer_alloc(layer_ctl);
  layer_setbuf(layer_mouse, buf_mouse, 16, 16, 99);
  init_mouse_cursor((char*)buf_mouse, 99);
  mouse_x = (binfo->screen_x - 16) / 2;
  mouse_y = (binfo->screen_y - 28 - 16) / 2;

  layer_cons = layer_alloc(layer_ctl);
  buf_cons = (unsigned char*)memory_manage_alloc_4k(memman, 256 * 165);
  layer_setbuf(layer_cons, buf_cons, 256, 165, -1);
  make_window(buf_cons, 256, 165, "console", 0);
  make_textbox8(layer_cons, 8, 28, 240, 128, COLOR_000000);
  task_cons = task_alloc();
  task_cons->tss.esp = memory_manage_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 8;
  task_cons->tss.eip = (int)&console_task;
  task_cons->tss.es = 1 * 8;
  task_cons->tss.cs = 2 * 8;
  task_cons->tss.ss = 1 * 8;
  task_cons->tss.ds = 1 * 8;
  task_cons->tss.fs = 1 * 8;
  task_cons->tss.gs = 1 * 8;
  *((int*)(task_cons->tss.esp + 4)) = (int)layer_cons;
  task_run(task_cons, 2, 2);

  layer_slide(layer_back, 0, 0);
  layer_slide(layer_cons, 32, 4);
  layer_slide(layer_window, 64, 56);
  layer_slide(layer_mouse, mouse_x, mouse_y);

  layer_updown(layer_back,     0);
  layer_updown(layer_cons,     1);
  layer_updown(layer_window,   2);
  layer_updown(layer_mouse,    3);

  lsprintf(str, "memory : %dMB    free:%dKB", memtotal/(1024*1024), memory_manage_total(memman)/1024);
  put_string_layer(layer_back, 0, 32, COLOR_FFFFFF, back_color, str, get_length(str));

  while(1){
    io_cli();
    if (!fifo_status32(&fifo)){
      task_sleep(task_a);
      io_sti();
    }
    else{
      i = get_fifo32(&fifo);
      io_sti();
      if(KEY_BOTTOM <= i && i < KEY_TOP){
        lsprintf(str, "%X", i - KEY_BOTTOM);
        put_string_layer(layer_back, 0, 16, COLOR_FFFFFF, back_color, str, get_length(str));
        if(i < KEY_BOTTOM + 0x54){
          if(keytable[i - KEY_BOTTOM] && cursor_x < 128){
            str[0] = keytable[i - KEY_BOTTOM];
            str[1] = '\0';
            put_string_layer(layer_window, cursor_x, 28, COLOR_000000, COLOR_C6C6C6, str, 1);
            cursor_x += 8;
          }
        }
        if(i == (256 + 0x0e) && cursor_x > 8){
          put_string_layer(layer_window, cursor_x, 28, COLOR_000000, COLOR_C6C6C6, " ", 1);
          cursor_x -= 8;
        }
        if(i == 256 + 0x0f){
          if(key_to == 0){
            key_to = 1;
            make_wtitle(buf_window, layer_window->bxsize, "task_a", 0);
            make_wtitle(buf_cons, layer_cons->bxsize, "console", 1);
          }
          else{
            key_to = 0;
            make_wtitle(buf_window, layer_window->bxsize, "task_a", 1);
            make_wtitle(buf_cons, layer_cons->bxsize, "console", 0);
          }
          layer_refresh(layer_window, 0, 0, layer_window->bxsize, 21);
          layer_refresh(layer_cons, 0, 0, layer_cons->bxsize, 21);
        }
        draw_rectangle((char*)buf_window, layer_window->bxsize, cursor_color, cursor_x, 28, 8, 16);
        layer_refresh(layer_window, cursor_x, 28, cursor_x + 8, 44);
      }
      else if(MOUSE_BOTTOM <= i && i < MOUSE_TOP){
        if(mouse_decode(&mdec, i - MOUSE_BOTTOM) != 0){
          lsprintf(str, "[lcr %d %d]", mdec.x, mdec.y);
          if(mdec.btn & 0x01){
            str[1] = 'L';
          }
          else if(mdec.btn & 0x02){
            str[3] = 'R';
          }
          else if(mdec.btn & 0x04){
            str[2] = 'C';
          }
          put_string_layer(layer_back, 32, 16, COLOR_FFFFFF, back_color, str, 10);
          mouse_x += mdec.x;
          mouse_y += mdec.y;
          if(mouse_x < 0){
            mouse_x = 0;
          }
          else if(mouse_x > binfo->screen_x - 1){
            mouse_x = binfo->screen_x - 1;
          }
          if(mouse_y < 0){
            mouse_y = 0;
          }
          else if(mouse_y > binfo->screen_y - 1){
            mouse_y = binfo->screen_y - 1;
          }
          layer_slide(layer_mouse, mouse_x, mouse_y);
          if(mdec.btn & 0x01){
            layer_slide(layer_window, mouse_x - 80, mouse_y - 8);
          }
        }
      }
      else if(i <= 1){
        if (i != 0){
          init_timer(timer, &fifo, 0);
          cursor_color = COLOR_C6C6C6;
        }
        else {
          init_timer(timer, &fifo, 1);
          cursor_color = COLOR_000000;
        }
        settimer(timer, 50);
        draw_rectangle((char*)layer_window->buf, layer_window->bxsize, cursor_color, cursor_x, 28, 8, 16);
        layer_refresh(layer_window, cursor_x, 28, cursor_x + 8, 44);
      }
    }
  }
}

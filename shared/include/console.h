#include "layer.h"
#include "../include/fifo.h"
#include "../include/mult_task.h"
#include "../include/timer.h"
#include "define.h"
#include "graphic.h"
#include "macro.h"
#include "memory.h"
#include "file.h"
#include "dsctbl_s.h"
#include "console_s.h"

void console_task(LAYER *layer, unsigned int memtotal);

void cons_newline(CONSOLE *cons);
void cons_runcmd(char *cmdline, CONSOLE *cons, int *fat, unsigned int memtotal);
void cmd_mem(CONSOLE *cons, unsigned int memtotal);
void cmd_clear(CONSOLE*cons);
void cmd_ls(CONSOLE *cons);
void cmd_cat(CONSOLE *cons, int *fat, char *cmdline);
int cmd_app(CONSOLE *cons, int *fat, char *cmdline);

int str_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
void cons_putchar(CONSOLE *cons, int chr, char move);
void cons_putstr(CONSOLE *cons, char *s);
void cons_putnstr(CONSOLE *cons, char *s, int n);


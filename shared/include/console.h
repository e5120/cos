#include "layer.h"
#include "../include/fifo.h"
#include "../include/mult_task.h"
#include "../include/timer.h"
#include "define.h"
#include "graphic.h"
#include "macro.h"
#include "memory.h"
#include "file_s.h"

void console_task(LAYER *layer, unsigned int memtotal);
int cons_newline(int cursor_y, LAYER *layer);
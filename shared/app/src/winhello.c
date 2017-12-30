#include "api.h"

char buf[150 * 50];

void HariMain(void){
  int win;
  win = api_open_window(buf, 150, 50, -1, "hello");
  api_rectangle_win(win, 8, 36, 136, 10, 3/*黄色*/);
  api_putstr_win(win, 28, 28, 0/*黒*/, 12, "hello window");
  api_end();
}
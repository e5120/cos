#include "api.h"

char buf[150 * 50];

void HariMain(void){
  int win;
  win = api_open_window(buf, 150, 50, -1, "hello");
  api_end();
}
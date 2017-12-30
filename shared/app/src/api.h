#pragma once

void api_putchar(int c);
void api_putstr(char *s);
void api_end(void);
int api_open_window(char *buf, int xsize, int ysize, int col_inv, char *title);
void api_putstr_win(int win, int x, int y, int col, int len, char *str);
void api_rectangle_win(int win, int x, int y, int width, int height, int col);
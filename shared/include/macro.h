#pragma once
// マクロ一覧
int lsprintf(char *str, const char *fmt, ...);  // stdio.hのsprintfに該当
unsigned int get_length(char* str); // 文字列の長さを返す
int lstrcmp(char *str1, char *str2);
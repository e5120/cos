#include "../include/macro.h"

void int2char(char *, int);
void int2hex(char *, int, int);
void int2dec(char *, int);
void int2str(char *, int);
void int2float(char *, int);
void strcls(char *str);

int lsprintf(char *str, const char *fmt, ...) {
	int *arg = (int *)(&str + 2);	// 可変個引数の配列
	int cnt;			// 生成した文字数
	int argc = 0;		// 引数の個数
	int i;				// カウンタ
	char buf[20];		// 数値変換用バッファ
	const char *p = fmt;// コピー元フォーマットを走査する

	strcls(str);
	strcls(buf);	// バッファの初期化
	for(cnt = 0; *p != '\0'; p++) {
		switch(*p) {
		case '%':
			// フォーマット指定子の場合は引数の数値を文字列へ変換
			switch(p[1]) {
				//case 'c': int2char(buf, arg[argc++]);	break;
				case 'd': int2dec(buf, arg[argc++]);	break;
				case 'x': int2hex(buf, arg[argc++], 0);	break;
				case 'X': int2hex(buf, arg[argc++], 1);	break;
				case 's': int2str(buf, arg[argc++]);	break;
				//case 'f':
			}
			// 変換した数値を生成文字列にコピー
			for(i = 0; buf[i] != '\0'; i++,cnt++) *str++ = buf[i];
			p++;
			break;

		case '\\':

			break;

		default:
			// フォーマット指定子以外はそのままコピー
			*str++ = *p; cnt++;
		}
	}
	return cnt;
}

// ヌル文字で埋める
void strcls(char *str) {
	while(*str != '\0') *str++ = '\0';
}

// ASCIIコードを文字に変換する
void int2char(char *s, int value) {
	s[0] = (char)value;
	s[1] = '\0';
}

// 数値を16進数文字列に変換する
// flag: 大文字なら1, 小文字なら0
void int2hex(char *s, int value, int flag) {
    int zero        = 0;    // 0以外の桁が出たかどうか
    int filter      = 0x0f; // 16進数で1桁だけ抽出するフィルタ
    int i, n;
    // 16進数のアルファベット
    char alphabet   = flag ? 'A' : 'a';

    // 16進数はint型で最大8桁
    for(i = 0; i < 8; i++) {
        n = value >> (7-i)*4;
        if(!zero && n != 0) zero = 1;   // 初めて0以外の桁が現れたとき
        else if(!zero)      continue;   // 0以外の桁が現れていないとき
        // 桁が10～15なら'A'～'F'に変換
        if((n & filter) >= 10) {
            *s++ = alphabet + (n & filter) - 10;
        } else {
            // '0'～'9'に変換
            *s++ = '0' + (n & filter);
        }
    }
    *s = '\0';
}
// 10進数valueのn桁目を返す
int figure(int value, int n) {
	int i;
	for(i = 0; i < n-1; i++) value /= 10;
	return value % 10;
}

// 数値を10進数文字列に変換する
void int2dec(char *s, int value) {
	int i;
	char zero = 1;

	if(!value) {
		s[0] = '0'; s[1] = '\0';
		return;
	}

	if(value < 0) {
		s[0] = '-'; s+= 1;
		value = -value;
	}

	for(i = 0; i < 10; i++) {
		if(zero && figure(value, 10-i) != 0) zero = 0;
		if(!zero) *s++ = '0' + figure(value, 10-i);
	}

	*s = '\0';
}

// 指定アドレスからバッファへの文字列のコピー(strcpyと同じ)
void int2str(char *s, int value) {
	char *p = (char *)value;
	while(*p != '\0') *s++ = *p++;
}

unsigned int get_length(char* str){
	int count = 0;
	while(str[count] != '\0'){
		++count;
	}
	return count;
}

int lstrcmp(char *str1, char *str2){
  while(*str1 == *str2){
    if(*str1 == '\0' && *str2 == '\0'){
      return 1;
    }
    ++str1;
    ++str2;
  }
  return 0;
}

int lstrncmp(char *str1, char *str2, int n){
	while(n != 0){
		if(*str1 != *str2){
			return 0;
		}
		--n;
		++str1;
		++str2;
	}
	return 1;
}


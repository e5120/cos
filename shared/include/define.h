#pragma once

// VRAMで用いる色の設定番号
#define COLOR_000000 0
#define COLOR_FF0000 1
#define COLOR_00FF00 2
#define COLOR_FFFF00 3
#define COLOR_0000FF 4
#define COLOR_FF00FF 5
#define COLOR_00FFFF 6
#define COLOR_FFFFFF 7
#define COLOR_C6C6C6 8
#define COLOR_840000 9
#define COLOR_008400 10
#define COLOR_848400 11
#define COLOR_000084 12
#define COLOR_840084 13
#define COLOR_008484 14
#define COLOR_848484 15

// バッファ最大値設定
#define STR_MAX_BUF 60
#define KEY_MAX_BUF 32
#define MOUSE_MAX_BUF 256
#define TIMER_MAX_BUF 8
#define FIFO_MAX_BUF 128

// 割り込みで用いられる各I/O機器の変数範囲
#define KEY_BOTTOM 256
#define KEY_TOP    512
#define MOUSE_BOTTOM 512
#define MOUSE_TOP  768
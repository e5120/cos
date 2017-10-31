[BITS 32]         ; 32bit modeの機械語

  GLOBAL    io_hlt   ; 関数名

[SECTION .text]   ; オブジェクトファイルではコレを書いてからプログラム記述
io_hlt :         ; void io_hlt(void);
  HLT
  RET
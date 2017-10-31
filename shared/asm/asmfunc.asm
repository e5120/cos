[BITS 32]         ; 32bit modeの機械語

  GLOBAL    io_hlt,write_mem8   ; 関数名

[SECTION .text]   ; オブジェクトファイルではコレを書いてからプログラム記述
io_hlt :         ; void io_hlt(void);
  HLT
  RET

write_mem8 :      ; void write_mem8(int addr, int data);
  MOV     ECX,[ESP+4]
  MOV     AL,[ESP+8]
  MOV     [ECX],AL
  RET

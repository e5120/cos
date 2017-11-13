[BITS 32]         ; 32bit modeの機械語
; 関数名
  GLOBAL    io_hlt,io_cli,io_sti,io_stihlt
  GLOBAL    io_in8, io_in16, io_in32
  GLOBAL    io_out8,io_out16,io_out32
  GLOBAL    io_load_eflags,io_store_eflags
  GLOBAL    load_gdtr,load_idtr
  GLOBAL    load_cr0,store_cr0
  GLOBAL    asm_interrupt_handler20,asm_interrupt_handler21
  GLOBAL    asm_interrupt_handler2c,asm_interrupt_handler27
  EXTERN    interrupt_handler20,interrupt_handler21
  EXTERN    interrupt_handler27,interrupt_handler2c
  GLOBAL    load_tr,farjmp


[SECTION .text]   ; オブジェクトファイルではコレを書いてからプログラム記述
io_hlt :         ; void io_hlt(void);
  HLT
  RET

io_cli :
  CLI
  RET

io_sti :
  STI
  RET

io_stihlt :
  STI
  HLT
  RET

io_in8 :
  MOV   EDX,[ESP+4]  ;port
  MOV   EAX,0
  IN    AL,DX
  RET

io_in16 :
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,0
  IN    AX,DX
  RET

io_in32 :
  MOV   EDX,[ESP+4] ;port
  IN    EAX,DX
  RET

io_out8 :
  MOV   EDX,[ESP+4] ;port
  MOV   AL,[ESP+8]  ;data
  OUT   DX,AL
  RET

io_out16 :
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,[ESP+8] ;data
  OUT   DX,AX
  RET

io_out32 :
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,[ESP+8] ;data
  OUT   DX,EAX
  RET

io_load_eflags :
  PUSHFD
  POP   EAX
  RET

io_store_eflags :
  MOV   EAX,[ESP+4] ;eflags
  PUSH  EAX
  POPFD
  RET

load_gdtr :
  MOV   AX,[ESP+4]
  MOV   [ESP+6],AX
  LGDT  [ESP+6]
  RET

load_idtr :
  MOV   AX,[ESP+4]
  MOV   [ESP+6],AX
  LIDT  [ESP+6]
  RET

asm_interrupt_handler20 :
  PUSH  ES
  PUSH  DS
  PUSHAD
  MOV   EAX,ESP
  PUSH  EAX
  MOV   AX,SS
  MOV   DS,AX
  MOV   ES,AX
  CALL  interrupt_handler20
  POP   EAX
  POPAD
  POP   DS
  POP   ES
  IRETD

asm_interrupt_handler21 :
  PUSH  ES
  PUSH  DS
  PUSHAD
  MOV   EAX,ESP
  PUSH  EAX
  MOV   AX,SS
  MOV   DS,AX
  MOV   ES,AX
  CALL  interrupt_handler21
  POP   EAX
  POPAD
  POP   DS
  POP   ES
  IRETD

asm_interrupt_handler27 :
  PUSH  ES
  PUSH  DS
  PUSHAD
  MOV   EAX,ESP
  PUSH  EAX
  MOV   AX,SS
  MOV   DS,AX
  MOV   ES,AX
  CALL  interrupt_handler27
  POP   EAX
  POPAD
  POP   DS
  POP   ES
  IRETD

asm_interrupt_handler2c :
  PUSH  ES
  PUSH  DS
  PUSHAD
  MOV   EAX,ESP
  PUSH  EAX
  MOV   AX,SS
  MOV   DS,AX
  MOV   ES,AX
  CALL  interrupt_handler2c
  POP   EAX
  POPAD
  POP   DS
  POP   ES
  IRETD

load_cr0 :
  MOV   EAX,CR0
  RET

store_cr0 :
  MOV   EAX,[ESP+4];
  MOV   CR0,EAX
  RET

load_tr :
  LTR   [ESP+4]
  RET

farjmp :
  JMP   FAR [ESP+4]
  RET
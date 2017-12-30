[BITS 32]         ; 32bit modeの機械語
; 関数名
  GLOBAL    io_hlt,io_cli,io_sti,io_stihlt
  GLOBAL    io_in8, io_in16, io_in32
  GLOBAL    io_out8,io_out16,io_out32
  GLOBAL    io_load_eflags,io_store_eflags
  GLOBAL    load_gdtr,load_idtr
  GLOBAL    load_cr0,store_cr0
  GLOBAL    memtest_sub
  GLOBAL    asm_interrupt_handler20,asm_interrupt_handler21
  GLOBAL    asm_interrupt_handler2c,asm_interrupt_handler27
  GLOBAL    asm_interrupt_handler0d,asm_interrupt_handler0c
  GLOBAL    load_tr,farjmp,farcall
  GLOBAL    asm_os_api
  GLOBAL    start_app,asm_end_app
  EXTERN    interrupt_handler20,interrupt_handler21
  EXTERN    interrupt_handler27,interrupt_handler2c
  EXTERN    interrupt_handler0d,interrupt_handler0c
  EXTERN    os_api


[SECTION .text]   ; オブジェクトファイルではコレを書いてからプログラム記述
io_hlt :         ; void io_hlt(void);
  HLT
  RET

io_cli :        ; void io_cli(void);
  CLI
  RET

io_sti :        ; void io_sti(void);
  STI
  RET

io_stihlt :     ; void io_stihlt(void);
  STI
  HLT
  RET

io_in8 :        ; int io_in8(int port);
  MOV   EDX,[ESP+4]  ;port
  MOV   EAX,0
  IN    AL,DX
  RET

io_in16 :       ; int io_in16(int port);
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,0
  IN    AX,DX
  RET

io_in32 :       ; int io_in32(int port);
  MOV   EDX,[ESP+4] ;port
  IN    EAX,DX
  RET

io_out8 :       ; void io_out8(int port, int data);
  MOV   EDX,[ESP+4] ;port
  MOV   AL,[ESP+8]  ;data
  OUT   DX,AL
  RET

io_out16 :       ; void io_out16(int port, int data);
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,[ESP+8] ;data
  OUT   DX,AX
  RET

io_out32 :       ; void io_out32(int port, int data);
  MOV   EDX,[ESP+4] ;port
  MOV   EAX,[ESP+8] ;data
  OUT   DX,EAX
  RET

io_load_eflags :  ; int io_load_eflags(void);
  PUSHFD
  POP   EAX
  RET

io_store_eflags : ; void io_store_eflags(int eflags);
  MOV   EAX,[ESP+4] ;eflags
  PUSH  EAX
  POPFD
  RET

load_gdtr :   ; void load_gdtr(int limit, int addr);
  MOV   AX,[ESP+4]
  MOV   [ESP+6],AX
  LGDT  [ESP+6]
  RET

load_idtr :   ; void load_idtr(int limit, int addr);
  MOV   AX,[ESP+4]
  MOV   [ESP+6],AX
  LIDT  [ESP+6]
  RET

load_cr0 :    ; int load_cr0(void);
  MOV   EAX,CR0
  RET

store_cr0 :   ; void store_cr0(int cr0);
  MOV   EAX,[ESP+4];
  MOV   CR0,EAX
  RET

load_tr :     ; void load_tr(int tr);
  LTR   [ESP+4]
  RET

asm_interrupt_handler20 :
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	interrupt_handler20
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  IRETD

asm_interrupt_handler21 :
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	interrupt_handler21
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  IRETD

asm_interrupt_handler27 :
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	interrupt_handler27
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  IRETD


asm_interrupt_handler2c :
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	interrupt_handler2c
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  IRETD

asm_interrupt_handler0d :
  STI
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	interrupt_handler0d
  CMP		EAX,0		; ���������Ⴄ
  JNE		asm_end_app		; ���������Ⴄ
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  ADD		ESP,4			; INT 0x0d �ł́A���ꂪ�K�v
  IRETD

asm_interrupt_handler0c :
  STI
  PUSH  ES
  PUSH  DS
  PUSHAD
  MOV   EAX,ESP
  PUSH  EAX
  MOV   AX,SS
  MOV   DS,AX
  MOV   ES,AX
  CALL  interrupt_handler0c
  CMP   EAX,0
  JNE   asm_end_app
  POP   EAX
  POPAD
  POP   DS
  POP   ES
  ADD   ESP,4
  IRETD

memtest_sub:	; unsigned int memtest_sub(unsigned int start, unsigned int end)
		PUSH	EDI						; �iEBX, ESI, EDI ���g�������̂Łj
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55			; pat0 = 0xaa55aa55;
		MOV		EDI,0x55aa55aa			; pat1 = 0x55aa55aa;
		MOV		EAX,[ESP+12+4]			; i = start;
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc				; p = i + 0xffc;
		MOV		EDX,[EBX]				; old = *p;
		MOV		[EBX],ESI				; *p = pat0;
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		EDI,[EBX]				; if (*p != pat1) goto fin;
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		ESI,[EBX]				; if (*p != pat0) goto fin;
		JNE		mts_fin
		MOV		[EBX],EDX				; *p = old;
		ADD		EAX,0x1000				; i += 0x1000;
		CMP		EAX,[ESP+12+8]			; if (i <= end) goto mts_loop;
		JBE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX				; *p = old;
		POP		EBX
		POP		ESI
		POP		EDI
		RET

farjmp :    ; void farjmp(int eip, int ct);
  JMP   FAR [ESP+4]
  RET

farcall :   ; void farcall(int eip, int ct);
  CALL  FAR [ESP+4]
  RET

asm_os_api :  ; void asm_os_api(void);
  STI
  PUSH  DS
  PUSH  ES
  PUSHAD    ; 保存のためのPUSH
  PUSHAD    ; os_apiを渡すためのPUSH
  MOV   AX,SS
  MOV   DS,AX       ; OS用のセグメントをDSとESにも入れる
  MOV   ES,AX
  CALL  os_api
  CMP   EAX,0       ; EAXが0でなければアプリ処理終了
  JNE   asm_end_app
  ADD   ESP,32
  POPAD
  POP   ES
  POP   DS
  IRETD

asm_end_app :
  MOV   ESP,[EAX]
  MOV   DWORD [EAX+4],0
  POPAD
  RET             ; cmd_appへ帰る

start_app :   ; void start_app(int eip, int cs, int esp, int ds);
  PUSHAD    ; 32bitレジスタを全部保存しておく
  MOV   EAX,[ESP+36]    ; アプリ用のEIP
  MOV   ECX,[ESP+40]    ; アプリ用のCS
  MOV   EDX,[ESP+44]    ; アプリ用のESP
  MOV   EBX,[ESP+48]    ; アプリ用のDS/SS
  MOV   EBP,[ESP+52]    ; tss.esp0の番地
  MOV   [EBP],ESP       ; OS用のESPを保存
  MOV   [EBP+4],SS      ; OS用のSSを保存

  MOV   ES,BX
  MOV   DS,BX
  MOV   FS,BX
  MOV   GS,BX
; 以下はRETFでアプリに行かせるためのスタック調整
  OR    ECX,3           ; アプリ用のセグメント番号に3をORする
  OR    EBX,3           ; アプリ用のセグメント番号に3をORする
  PUSH  EBX             ; アプリのSS
  PUSH  EDX             ; アプリのESP
  PUSH  ECX             ; アプリのCS
  PUSH  EAX             ; アプリのEIP
  RETF
; アプリが終了してもここには帰ってこない









;haribote-os;

;BOOT_INFO
CYLS  EQU   0x0ff0      ; ブートセクタが設定
LEDS  EQU   0x0ff1
VMODE EQU   0x0ff2      ; 色数に関する情報。何ビットカラーか？
SCRNX EQU   0x0ff4      ; 解像度：X
SCRNY EQU   0x0ff6      ; 解像度：Y
VRAM  EQU   0x0ff8      ; グラフィックバッファの開始番地

  ORG   0xc200    ; プログラム開始番地

  MOV   AL,0x13   ; VGAグラフィックス 320x200x8bit color

  MOV   AH,0x00
  INT   0x10
  MOV   BYTE [VMODE],8    ; 画面モードをメモ
  MOV   WORD [SCRNX],320
  MOV   WORD [SCRNY],200
  MOV   DWORD [VRAM], 0x000a0000

; キーボードのLED状態をBIOSから取得
  MOV   AH,0x02
  INT   0x16
  MOV   [LEDS],AL


fin:
  HLT
  JMP fin

#pragma once

// アセンブリ側で設定した諸変数のアドレスと定数
#define ADDR_BOOTINFO   0x00000ff0
#define ADDR_DISKIMG    0x00100000
#define ADDR_IDT        0x0026f800
#define LIMIT_IDT       0x000007ff
#define ADDR_GDT        0x00270000
#define LIMIT_GDT       0x0000ffff
#define ADDR_BOOTPAK    0x00280000
#define LIMIT_BOOTPAK   0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a
#define AR_INTGATE32    0x008e
#define AR_TSS32        0x0089


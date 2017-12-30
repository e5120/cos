#pragma once
// asumfunc.asm内の関数
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int  io_in8(int port);
void io_out8(int port, int data);
int  io_load_eflags(void);
void io_store_eflags(int eflags);
int  load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_end_app(void);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void asm_interrupt_handler20(void);
void asm_interrupt_handler21(void);
void asm_interrupt_handler27(void);
void asm_interrupt_handler2c(void);
void asm_interrupt_handler0d(void);
void asm_interrupt_handler0c(void);

// original application
void asm_os_api(void);
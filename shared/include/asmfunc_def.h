// asumfunc.asm内の関数
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
int load_cr0(void);
void store_cr0(int cr0);
void load_gdtr(short, int);
void load_idtr(short, int);
void asm_interrupt_handler20(void);
void asm_interrupt_handler21(void);
void asm_interrupt_handler27(void);
void asm_interrupt_handler2c(void);
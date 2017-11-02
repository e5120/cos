// asmhead.asmのBOOT_INFO参照
typedef struct BOOTINFO{
  char  cyls;       // 0x0ff0 (1byte)
  char  leds;       // 0x0ff1 (1byte)
  char  vmode;      // 0x0ff2 (1byte)
  char  reserve;    // 0x0ff3 (使われてない)
  short screen_x;   // 0x0ff4 (2byte = short型)
  short screen_y;   // 0x0ff6 (2byte = short型)
  char* vram;       // 0x0ff8 (1byte)
}BOOT_INFO;

// asumfunc.asm内の関数
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(short, int);
void load_idtr(short, int);

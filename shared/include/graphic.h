// プロトタイプ宣言
void init_desktop(unsigned char* vram_addr, int screen_x, int screen_y);
void init_palette(void);
void set_palette(int color, unsigned char *rgb);
void set_background(unsigned char* vram_addr, int screen_x, int screen_y, unsigned char color);
void draw_rectangle(unsigned char* vram_addr, int screen_width, unsigned char color,
                    int lx, int ly, int width, int height);
void put_font(char* vram_addr, int screen_x, int x, int y, char color, char* font);
void put_string(char* vram_addr, int screen_x, int x, int y, char color, char* str);
void init_mouse_cursor(char *mouse, char bc);
void put_block(char* vram, int screen_x, int width, int height,
              int x, int y, char* buf, int width_per_line);

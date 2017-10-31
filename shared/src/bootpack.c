void io_hlt(void);

void HariMain(void){
  int i;
  for(i = 0xa0000; i <= 0xaffff; ++i){
    write_mem8(i, 14);
  }
  while(1){
    io_hlt();
  }
}

void _start(void){
  HariMain();
}

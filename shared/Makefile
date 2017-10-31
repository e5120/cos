HOME = /home/tetsuya
ASM = ./asm/
BIN = ./bin/
SYS = ./sys/
IMAGE = ./image/
SRC = ./src/
LIST = ./list/
OBJ = ./obj/

INPUT = cos
OS = haribote
BOOT = asmhead

default :
	make build

ipl : $(ASM)ipl.asm Makefile
	nasm -f bin $(ASM)ipl.asm -o $(BIN)ipl.bin -l $(LIST)ipl.lst

boot :	$(ASM)$(BOOT).asm Makefile
	nasm -f bin $(ASM)$(BOOT).asm -o $(BIN)$(BOOT).bin -l $(LIST)$(BOOT).lst

func :
#	nasm -f win32 $(ASM)asmfunc.asm -o $(OBJ)asmfunc.obj
	nasm -f elf32 $(ASM)asmfunc.asm -o $(OBJ)asmfunc.o

gcc :
	gcc -m32 -nostdlib -Wl,--oformat=binary $(OBJ)asmfunc.o $(SRC)bootpack.c -T os.lds -o $(BIN)bootpack.bin
#	gcc -m32 -nostdlib -Wl,--oformat=binary $(OBJ)asmfunc.o $(SRC)bootpack.c -o $(BIN)bootpack.bin
#	gcc -O2 -m32 -march=i686 -nostdlib -Wl,--oformat=binary $(SRC)bootpack.c $(OBJ)asmfunc.o -o $(BIN)bootpack.bin

append :
	cat $(BIN)$(BOOT).bin $(BIN)bootpack.bin > $(SYS)$(OS).sys
	mformat -f 1440 -C -B $(BIN)ipl.bin -i $(IMAGE)$(OS).img ::
	mcopy -i $(IMAGE)$(OS).img $(SYS)$(OS).sys ::
#	mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B $(BIN)ipl.bin -i $(SYS)$(OS).sys
#	mcopy -i $(IMAGE)$(OS).img $(SYS)$(OS).sys

run_os :
	qemu-system-i386 -m 32 -localtime -vga std -fda $(IMAGE)$(OS).img -monitor stdio

build :
	make ipl
	make boot
	make func
	make gcc
	make append

img : $(BIN)ipl.bin Makefile
	cat $(BIN)ipl.bin > $(IMAGE)$(INPUT).img

run_ipl :
	make img
	qemu-system-i386 -m 32 -localtime -vga std -fda $(IMAGE)$(INPUT).img -monitor stdio

clean :
	rm $(BIN)*
	rm $(IMAGE)*
	rm $(LIST)*
	rm $(OBJ)*
	rm $(SYS)*

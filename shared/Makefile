HOME = /home/tetsuya
TOOLS = $(HOME)/z_tools
ASM = ./asm/
BIN = ./bin/
SYS = ./sys/
IMAGE = ./image/
SRC = ./src/
LIST = ./list/

INPUT = cos
OS_NAME = haribote

default :
	make img

ipl : $(ASM)ipl.asm Makefile
	nasm $(ASM)ipl.asm -o $(BIN)ipl.bin -l $(LIST)ipl.lst

tail : $(ASM)tail.asm Makefile
	nasm $(ASM)tail.asm -o $(BIN)tail.bin -l $(LIST)tail.lst

assemble :
	nasm $(ASM)$(INPUT).asm -o $(IMAGE)$(INPUT).img -l $(LIST)$(INPUT).lst

os :
	make ipl
	nasm -f bin $(ASM)$(OS_NAME).asm -o $(SYS)$(OS_NAME).sys
	mformat -f 1440 -C -B $(BIN)ipl.bin -i $(IMAGE)$(OS_NAME).img ::
	mcopy -i $(IMAGE)$(OS_NAME).img $(SYS)$(OS_NAME).sys ::

img : $(BIN)ipl.bin Makefile
	cat $(BIN)ipl.bin > $(IMAGE)$(INPUT).img

run :
	make img
	qemu-system-i386 -m 32 -localtime -curses -vga std -fda $(IMAGE)$(INPUT).img -monitor stdio

clean :
	rm $(IMAGE)*
	rm $(LIST)*
	rm $(BIN)*
	rm $(SYS)*

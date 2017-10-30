HOME = /home/tetsuya
TOOLS = $(HOME)/z_tools
ASM = ./asm/
BIN = ./bin/
IMAGE = ./image/
SRC = ./src/
LIST = ./list/

INPUT = cos

default :
	make img

ipl : $(ASM)ipl.asm Makefile
	nasm $(ASM)ipl.asm -o $(BIN)ipl.bin -l $(LIST)ipl.lst

tail : $(ASM)tail.asm Makefile
	nasm $(ASM)tail.asm -o $(BIN)tail.bin -l $(LIST)tail.lst

assemble :
	nasm $(ASM)$(INPUT).asm -o $(IMAGE)$(INPUT).img -l $(LIST)$(INPUT).lst

img : $(BIN)ipl.bin Makefile
	cat $(BIN)ipl.bin > $(IMAGE)$(INPUT).img

run :
	make img
	qemu-system-i386 -curses -fda $(IMAGE)$(INPUT).img

clean :
	rm $(IMAGE)*
	rm $(LIST)*
	rm $(BIN)*

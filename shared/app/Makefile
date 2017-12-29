NASM = nasm
BFLAGS = -f bin
OFLAGS = -f elf32

SRC = $(wildcard $(SRCDIR)/*.c)
ASM = $(ASMDIR)/hello.asm $(ASMDIR)/hello0.asm

CC = gcc
CFLAGS = -m32 -march=i686 -nostdlib

LD = ld
LFLAGS = -m elf_i386 --oformat=binary -T app.lds

SRCDIR = ./src
ASMDIR = ./asm
BINDIR = ./bin
OBJDIR = ./obj

all: api $(SRC) $(ASM)

api:
	$(NASM) $(OFLAGS) $(ASMDIR)/api.asm -o $(OBJDIR)/api.o

$(SRCDIR)/%.c:
	$(CC) $(CFLAGS) $(SRCDIR)/$*.c -c -o $(OBJDIR)/$*.o
	$(LD) $(LFLAGS) $(OBJDIR)/$*.o $(OBJDIR)/api.o -o $(BINDIR)/$*.bin

$(ASMDIR)/%.asm:
	$(NASM) $(OFLAGS) $(ASMDIR)/$*.asm -o $(OBJDIR)/$*.o
	$(LD)	$(LFLAGS) $(OBJDIR)/$*.o -o $(BINDIR)/$*.bin

clean:
	rm $(OBJDIR)/*.o
	rm $(BINDIR)/*.bin

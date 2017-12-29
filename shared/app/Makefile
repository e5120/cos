CC = gcc
CFLAGS = -m32 -march=i686 -nostdlib

LD = ld
LFLAGS = -m elf_i386 --oformat=binary -T app.lds

NASM = nasm
BFLAGS = -f bin
OFLAGS = -f elf32

ASMDIR = ./asm
BINDIR = ./bin
SRCDIR = ./src
OBJDIR = ./obj

all: api app crack hello

api: $(ASMDIR)/api.asm
	$(NASM) $(OFLAGS) $(ASMDIR)/api.asm -o $(OBJDIR)/api.o

app: $(OBJDIR)/api.o Makefile
	$(CC) $(CFLAGS) $(SRCDIR)/a.c -c -o $(OBJDIR)/a.o
	$(LD) $(LFLAGS) $(OBJDIR)/a.o $(OBJDIR)/api.o -o $(BINDIR)/a.bin

crack:
	$(NASM) $(BFLAGS) $(ASMDIR)/crack.asm -o $(BINDIR)/crack.bin

hlt:
	$(NASM) $(BFLAGS) $(ASMDIR)/hlt.asm -o $(BINDIR)/hlt.bin

hello:
	$(NASM) $(BFLAGS) $(ASMDIR)/hello.asm -o $(BINDIR)/hello.bin
	$(NASM) $(BFLAGS) $(ASMDIR)/hello0.asm -o $(BINDIR)/hello0.bin

clean:
	rm $(BINDIR)/*.bin
	rm $(OBJDIR)/*.o
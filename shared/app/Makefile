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

all: api app

api: $(ASMDIR)/api.asm
	$(NASM) $(OFLAGS) $(ASMDIR)/api.asm -o $(OBJDIR)/api.o

app: $(OBJDIR)/api.o Makefile
	$(CC) $(CFLAGS) $(SRCDIR)/a.c -c -o $(OBJDIR)/a.o
	$(LD) $(LFLAGS) $(OBJDIR)/a.o $(OBJDIR)/api.o -o $(BINDIR)/a.bin

hlt:
	$(NASM) $(BFLAGS) hlt.asm -o hlt.bin

hello:
	$(NASM) $(BFLAGS) hello.asm -o hello.bin
	$(NASM) $(BFLAGS) hello0.asm -o hello0.bin

clean:
	rm $(BINDIR)/*.bin
	rm $(OBJDIR)/*.o
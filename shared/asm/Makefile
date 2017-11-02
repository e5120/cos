NASM = nasm
BFLAGS = -f bin
OFLAGS = -f elf32

BINDIR = ../bin
LISTDIR = ../list
OBJDIR = ../obj

ASM_FUNC = asmfunc

all : *.asm func

%.asm : Makefile
	$(NASM) $(BFLAGS) $*.asm -o $(BINDIR)/$*.bin -l $(LISTDIR)/$*.lst

func :
	$(NASM) $(OFLAGS) $(ASM_FUNC).asm -o $(OBJDIR)/$(ASM_FUNC).o

clean :
	rm $(BINDIR)/*.bin
	rm $(LISTDIR)/*.lst

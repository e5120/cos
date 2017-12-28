CC = gcc
CFLAGS = -m32 -march=i686 -nostdlib
LD = ld
LFLAGS = -m elf_i386 -M=os.map --oformat=binary -T os.lds
INCLUDE = -I../include
SRC = $(wildcard *.c)
OBJ = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
BINDIR = ../bin
OBJDIR = ../obj

TARGET = bootpack

all :	$(TARGET)

$(TARGET) : $(OBJ) Makefile
	$(LD) $(LFLAGS) $(OBJ) $(OBJDIR)/asmfunc.o -o $(BINDIR)/$(TARGET).bin

$(OBJDIR)/%.o : %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) $*.c -c -o $(OBJDIR)/$*.o

clean :
	rm $(OBJDIR)/*.o
	rm os.map

MAKE = make --no-print-directory

QEMU = qemu-system-i386
QFLAGS = -m 32 -localtime -vga std -fda
QFLAGS2 = -monitor stdio

ASMDIR = ./asm
BINDIR = ./bin
IMGDIR = ./image
OBJDIR = ./obj
SRCDIR = ./src

TARGET = haribote.img

all :	$(TARGET)

$(TARGET) :	assembly compile append

assembly :
	$(MAKE) -B -C $(ASMDIR)

append :
	$(MAKE) -C $(BINDIR)

compile :
	$(MAKE) -C $(SRCDIR)

run :
	$(QEMU) $(QFLAGS) $(IMGDIR)/$(TARGET) $(QFLAGS2)

clean :
	$(MAKE) -C $(ASMDIR) clean
	$(MAKE) -C $(SRCDIR) clean
	$(MAKE) -C $(BINDIR) clean


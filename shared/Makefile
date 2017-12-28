MAKE = make --no-print-directory
MFLAG = -C

QEMU = qemu-system-i386
QFLAGS = -L . -m 32 -localtime -vga std -fda
QFLAGS2 = -monitor stdio

VBM = VBoxManage
STVM = startvm
CTLVM = controlvm
VMTARGET = original

APPDIR = ./app
ASMDIR = ./asm
BINDIR = ./bin
IMGDIR = ./image
OBJDIR = ./obj
SRCDIR = ./src

TARGET = haribote.img

all :	$(TARGET)

$(TARGET) :	application assembly compile append

assembly :
	$(MAKE) -B $(MFLAG) $(ASMDIR)

append :
	$(MAKE) $(MFLAG) $(BINDIR)

compile :
	$(MAKE) $(MFLAG) $(SRCDIR)

application :
	$(MAKE) -B $(MFLAG) $(APPDIR)

run :
	$(QEMU) $(QFLAGS) $(IMGDIR)/$(TARGET) $(QFLAGS2)

start :
	$(VBM) $(STVM) $(VMTARGET)

reset :
	$(VBM) $(CTLVM) $(VMTARGET) resume
	$(VBM) $(CTLVM) $(VMTARGET) reset

pause :
	$(VBM) $(CTLVM) $(VMTARGET) pause

stop :
	$(VBM) $(CTLVM) $(VMTARGET) poweroff

clean :
	$(MAKE) $(MFLAG) $(ASMDIR) clean
	$(MAKE) $(MFLAG) $(SRCDIR) clean
	$(MAKE) $(MFLAG) $(BINDIR) clean


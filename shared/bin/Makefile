FORMAT = mformat
FFLAGS = -f 1440 -C -B
COPY = mcopy
CFLAGS =

SYSDIR = ../sys
FILEDIR = ../file
IMGDIR = ../image

IPL = ipl.bin
BOOT = asmhead.bin bootpack.bin
OS = haribote

append : $(BOOT) $(IPL) Makefile
	cat $(BOOT) > $(SYSDIR)/$(OS).sys
	$(FORMAT) $(FFLAGS) $(IPL) -i $(IMGDIR)/$(OS).img ::
	$(COPY) -i $(IMGDIR)/$(OS).img $(SYSDIR)/$(OS).sys ::
	$(COPY) -i $(IMGDIR)/$(OS).img $(FILEDIR)/test.asm ::
	$(COPY) -i $(IMGDIR)/$(OS).img $(FILEDIR)/sample.c ::


clean :
	rm $(IMGDIR)/*.img
	rm $(SYSDIR)/*.sys

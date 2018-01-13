LDFLAGS += $(shell pkg-config --libs x11 xi)
LDFLAGS += -lev

XMousePasteBlock : xmousepasteblock.c
	gcc $(CFLAGS) $(LDFLAGS) xmousepasteblock.c -o xmousepasteblock

prefix=/usr/local

debug:
	$(MAKE) xmousepasteblock "CFLAGS=-g -DDEBUG"

clean:
	rm xmousepasteblock

install:
	install -m 0755 xmousepasteblock $(prefix)/bin

uninstall:
	rm $(prefix)/bin/xmousepasteblock

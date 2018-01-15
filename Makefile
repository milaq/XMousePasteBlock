TARGET = xmousepasteblock

INSTALL = install
PREFIX = /usr
BINDIR = $(PREFIX)/bin

CC = gcc
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wundef -Wshadow -Wformat-security

LD = $(CC)
LDFLAGS += $(shell pkg-config --libs x11 xi)
LDFLAGS += -lev

.NOTPARALLEL:

.PHONY: all
all: clean $(TARGET)

.PHONY: debug
debug: clean
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: $(TARGET)
$(TARGET): $(TARGET).o
	$(LD) "$<" $(LDFLAGS) -o "$(TARGET)"

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -o "$@" -c "$<"

.PHONY: install
install: $(TARGET)
	$(INSTALL) -Dm 0755 "$(TARGET)" "$(DESTDIR)$(BINDIR)/$(TARGET)"

.PHONY: uninstall
uninstall:
	$(RM) "$(DESTDIR)$(BINDIR)/$(TARGET)"

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TARGET).o

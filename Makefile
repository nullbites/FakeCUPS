CFLAGS=-w

all:
	gcc cups.c -o cupsd $(CFLAGS)

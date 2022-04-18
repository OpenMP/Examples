CC=gcc
CFLAGS=-O

default: chk_tags.x

chk_tags.x: chk_tags.c
	$(CC) -o $@ $(CFLAGS) $?

clean:
	-rm -f *.o *.x

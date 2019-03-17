CC=gcc
LIBS=-lpthread

pacman: pacman.c
	$(CC) -o $@ $< $(LIBS)

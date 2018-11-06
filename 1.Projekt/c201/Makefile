
PRJ=IAL
#
PROGS=$(PRJ)-c201
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-c201: $(PRJ).c $(PRJ)-c201.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-c201.c

clean:
	rm -f *.o *.out $(PROGS)
#
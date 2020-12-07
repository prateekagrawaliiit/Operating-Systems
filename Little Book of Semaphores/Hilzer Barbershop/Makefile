CC=gcc
CFLAGS=-Wall -pthread

all: noMutex

queue.o: queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c -o queue.o

noMutex: queue.o noMutex.c
	$(CC) $(CFLAGS) noMutex.c queue.o -o noMutex
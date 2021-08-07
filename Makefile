
CC=gcc
CFLAGS=-Wall -g

all: mypopen

mypopen: main.o mypopen.o mypclose.o
	$(CC) -o mypopen main.o mypopen.o mypclose.o

main.o: main.c
	$(CC) -c main.c

mypopen.o: mypopen.c
	$(CC) -c mypopen.c

mypclose.o: mypclose.c
	$(CC) -c mypclose.c


clean:
	rm mypopen main.o mypopen.o mypclose.o




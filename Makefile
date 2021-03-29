CFLAGS=-c -Wall -O0

all: icopack

icopack: main.o
	$(CC) main.o -o icopack

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf main.o icopack

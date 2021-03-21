CFLAGS=-c -Wall -O0 

all: icon_tool

icon_tool: main.o 
	$(CC) main.o -o icon_tool 

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf main.o icon_tool

CC = gcc
CFLAGS = -Wall -g -std=c11 -lm

all: build

build: main

main: main.c
	$(CC) main.c -o main $(CFLAGS)

run: build
	./main
val: build
	valgrind --leak-check=full ./main

clean:
	rm -f *.o main

.PHONY: clean run build

CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all -Wextra

all: d8sh

clean:
	rm -f d8sh ./*.o

d8sh: d8sh.o executor.o parser.tab.o lexer.o
	gcc -lreadline d8sh.o executor.o parser.tab.o lexer.o -o d8sh

d8sh.o: d8sh.c executor.h lexer.h
	gcc $(CFLAGS) -c d8sh.c

executor.o: executor.c executor.h command.h
	gcc $(CFLAGS) -c executor.c

parser.tab.o: parser.tab.c command.h
	gcc $(CFLAGS) -c parser.tab.c

lexer.o: lexer.c
	gcc $(CFLAGS) -c lexer.c

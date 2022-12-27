CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all 
PROGS = public01 public02 public03 public04

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) a.out

$(PROGS): photoalbum.o my_memory_checker_216.o 
public%: public%.o

photoalbum.o: photoalbum.h
my_memory_checker_216.o: my_memory_checker_216.h
public%.o: photoalbum.h my_memory_checker_216.h 


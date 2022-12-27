CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all -Wextra
PROGS = public01 public02 public03 public04 public05 student_tests

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) a.out

$(PROGS): calendar.o my_memory_checker_216.o
public%: public%.o

calendar.o: calendar.h event.h
my_memory_checker_216.o: my_memory_checker_216.h
public%.o: calendar.h my_memory_checker_216.h 
student_tests.o: calendar.h my_memory_checker_216.h
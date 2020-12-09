GCC = gcc
CFLAGS = -std=c99 -Wall
OBJS = functions.o main.o


debug: CFLAGS += -g -Og
debug: all

release: CFLAGS += -O1
release: all

all: program

program: $(OBJS)
	$(GCC) $(CFLAGS) -o program $(OBJS)

main.o: main.c
	$(GCC) $(CFLAGS) -c -o main.o main.c

functions.o: functions.c
	$(GCC) $(CFLAGS) -c -o functions.o functions.c

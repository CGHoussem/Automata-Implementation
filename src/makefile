GCC = gcc
CFLAGS = -std=c99 -Wall
OBJS = functions.o extra_func.o main.o

release: CFLAGS += -O1 -D DEBUG=0
release: all

debug: CFLAGS += -g -Og -D DEBUG=1
debug: all

all: program

program: $(OBJS)
	$(GCC) $(CFLAGS) -o program $(OBJS)

main.o: main.c
	$(GCC) $(CFLAGS) -c -o main.o main.c

functions.o: functions.c
	$(GCC) $(CFLAGS) -c -o functions.o functions.c

extra_func.o: extra_func.c
	$(GCC) $(CFLAGS) -c -o extra_func.o extra_func.c

clean:
	rm -rf $(OBJS) program

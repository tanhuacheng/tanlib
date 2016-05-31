cflags = -Wall --std gnu99 -I include/

all : tmemfill.out tqueue-array.out

tmemfill.out : memfill.o tmemfill.o
	cc $(cflags) -o tmemfill.out memfill.o tmemfill.o
tqueue-array.out : memfill.o queue-array.o tqueue-array.o
	cc $(cflags) -o tqueue-array.out memfill.o queue-array.o tqueue-array.o -lpthread

# ./src/
memfill.o : src/memfill.c
	cc $(cflags) -c src/memfill.c
queue-array.o : src/queue-array.c
	cc $(cflags) -c src/queue-array.c

# ./test/
tmemfill.o : test/tmemfill.c
	cc $(cflags) -c test/tmemfill.c
tqueue-array.o : test/tqueue-array.c
	cc $(cflags) -c test/tqueue-array.c

#clean
.PHONY : clear
clear :
	-rm *.o \
		tmemfill.out tqueue-array.out 
.PHONY : clean
clean :
	-rm *.o


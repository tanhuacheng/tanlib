CC 		= gcc
CFLAGS 	= -g -Wall --std gnu99 -I ./include/
LDFLAGS = -lpthread
VPATH 	= ./src/ : ./test/ : ./include/

tmemfill_obj 		= memfill.o tmemfill.o
tqueue_array_obj 	= memfill.o queue-array.o tqueue-array.o
tjitter_obj			= memfill.o queue-array.o jitter.o tjitter.o

all : tmemfill.out tqueue-array.out tjitter.out

tmemfill.out : $(tmemfill_obj)
	$(CC) $(CFLAGS) -o tmemfill.out $(tmemfill_obj) $(LDFLAGS)
tqueue-array.out : $(tqueue_array_obj)
	$(CC) $(CFLAGS) -o tqueue-array.out $(tqueue_array_obj) $(LDFLAGS)
tjitter.out : $(tjitter_obj)
	$(CC) $(CFLAGS) -o tjitter.out $(tjitter_obj) $(LDFLAGS)

-include .depends.d

config:
	-./.configure.sh

clean:
	-rm *.o *.out


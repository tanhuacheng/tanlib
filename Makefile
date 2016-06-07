CC 		= gcc
CFLAGS 	= -g -Wall --std gnu99 -I ./include/
LDFLAGS = -lpthread
VPATH 	= ./src/ : ./test/ : ./include/

tmemfill_obj 		= memfill.o tmemfill.o
tqueue_array_obj 	= memfill.o queue-array.o tqueue-array.o
tjitter_obj			= memfill.o queue-array.o jitter.o tjitter.o
tqueue_list_obj		= queue-list.o tqueue-list.o
tqueue_link_obj 	= queue-link.o tqueue-link.o
tqueue_list_vs_link_obj = queue-list.o queue-link.o tqueue-list-vs-link.o

all : tmemfill.out tqueue-array.out tjitter.out tqueue-list.out tqueue-link.out \
	tqueue-list-vs-link.out

tmemfill.out : $(tmemfill_obj)
	$(CC) $(CFLAGS) -o tmemfill.out $(tmemfill_obj) $(LDFLAGS)
tqueue-array.out : $(tqueue_array_obj)
	$(CC) $(CFLAGS) -o tqueue-array.out $(tqueue_array_obj) $(LDFLAGS)
tjitter.out : $(tjitter_obj)
	$(CC) $(CFLAGS) -o tjitter.out $(tjitter_obj) $(LDFLAGS)
tqueue-list.out : $(tqueue_list_obj)
	$(CC) $(CFLAGS) -o tqueue-list.out $(tqueue_list_obj) $(LDFLAGS)
tqueue-link.out : $(tqueue_link_obj)
	$(CC) $(CFLAGS) -o tqueue-link.out $(tqueue_link_obj) $(LDFLAGS)
tqueue-list-vs-link.out : $(tqueue_list_vs_link_obj)
	$(CC) $(CFLAGS) -o tqueue-list-vs-link.out $(tqueue_list_vs_link_obj) $(LDFLAGS)

-include .depends.d

config:
	-./.configure.sh

clean:
	-rm *.o *.out


CC 		= gcc
CFLAGS 	= -g -Wall --std gnu99 -I ./include/
LDFLAGS = -pthread
VPATH 	= ./src/ : ./test/ : ./include/

tmemfill_obj 		= memfill.o tmemfill.o
tqueue_array_obj 	= memfill.o queue-array.o tqueue-array.o
tjitter_obj			= memfill.o queue-array.o jitter.o tjitter.o
tqueue_list_obj		= queue-list.o tqueue-list.o
tqueue_link_obj 	= queue-link.o tqueue-link.o
tqueue_list_vs_link_obj = queue-list.o queue-link.o tqueue-list-vs-link.o
tbstree_obj			= tbstree.o bstree.o
tforeach_obj 		= tforeach.o
tsort_obj 			= insert-sort.o merge-sort.o tsort.o
tmax_subarray_obj 	= max-subarray.o tmax-subarray.o
tzero_cross_obj		= zero-cross.o tzero-cross.o
tlog_obj			= log.o tlog.o

all : tmemfill.out tqueue-array.out tjitter.out tqueue-list.out tqueue-link.out \
	  tqueue-list-vs-link.out tbstree.out tforeach.out tsort.out tmax-subarray.out \
	  tzero-cross.out tlog.out

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
tbstree.out : $(tbstree_obj)
	$(CC) $(CFLAGS) -o tbstree.out $(tbstree_obj) $(LDFLAGS)
tforeach.out : $(tforeach_obj)
	$(CC) $(CFLAGS) -o tforeach.out $(tforeach_obj) $(LDFLAGS)
tsort.out : $(tsort_obj)
	$(CC) $(CFLAGS) -o tsort.out $(tsort_obj)
tmax-subarray.out : $(tmax_subarray_obj)
	$(CC) $(CFLAGS) -o tmax-subarray.out $(tmax_subarray_obj)
tzero-cross.out : $(tzero_cross_obj)
	$(CC) $(CFLAGS) -o tzero-cross.out $(tzero_cross_obj) -lm
tlog.out : $(tlog_obj)
	$(CC) $(CFLAGS) -o tlog.out $(tlog_obj)

-include .depends.d

config:
	-./.configure.sh

clean:
	-rm *.o *.out


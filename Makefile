CC 		= gcc
CFLAGS 	= -Wall --std gnu99 -I ./include/
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



# 这部分使用make depends(需要把下面注释的位置打开)自动生成
jitter.o: src/jitter.c include/queue-array.h include/jitter.h
memfill.o: src/memfill.c include/memfill.h
queue-array.o: src/queue-array.c include/memfill.h include/queue-array.h
tjitter.o: test/tjitter.c include/jitter.h
tmemfill.o: test/tmemfill.c include/memfill.h
tqueue-array.o: test/tqueue-array.c include/queue-array.h



#sources = $(wildcard *.c src/*.c test/*.c)
#include $(sources:.c=.d)
#%.d: %.c
#	set -e; rm -f $@; \
#	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$
#depends.d:
#	cat $(sources:.c=.d) > depends.d
#	rm $(sources:.c=.d)
#clean :
#	rm $(sources:.c=.o) $(sources:.c=.d)



clean:
	rm *.o *.out


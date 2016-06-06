jitter.o: src/jitter.c include/queue-array.h include/jitter.h
memfill.o: src/memfill.c include/memfill.h
queue-array.o: src/queue-array.c include/memfill.h include/queue-array.h
queue-list.o: src/queue-list.c include/queue-list.h
tjitter.o: test/tjitter.c include/jitter.h
tmemfill.o: test/tmemfill.c include/memfill.h
tqueue-array.o: test/tqueue-array.c include/queue-array.h
tqueue-list.o: test/tqueue-list.c include/queue-list.h

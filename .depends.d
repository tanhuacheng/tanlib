bstree.o: src/bstree.c include/bstree.h include/typedefs.h
insert-sort.o: src/insert-sort.c include/sort.h include/typedefs.h
jitter.o: src/jitter.c include/queue-array.h include/jitter.h
memfill.o: src/memfill.c include/memfill.h
merge-sort.o: src/merge-sort.c include/sort.h include/typedefs.h
queue-array.o: src/queue-array.c include/memfill.h include/queue-array.h
queue-link.o: src/queue-link.c include/queue-link.h
queue-list.o: src/queue-list.c include/queue-list.h
tbstree.o: test/tbstree.c include/bstree.h include/typedefs.h
tforeach.o: test/tforeach.c include/foreach.h
tjitter.o: test/tjitter.c include/jitter.h
tmemfill.o: test/tmemfill.c include/memfill.h
tqueue-array.o: test/tqueue-array.c include/queue-array.h
tqueue-link.o: test/tqueue-link.c include/queue-link.h
tqueue-list-vs-link.o: test/tqueue-list-vs-link.c include/queue-list.h \
 include/queue-link.h
tqueue-list.o: test/tqueue-list.c include/queue-list.h
tsort.o: test/tsort.c include/foreach.h include/sort.h include/typedefs.h

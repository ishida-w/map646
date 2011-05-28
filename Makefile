OBJS	= map646.o mapping.o tunif.o checksum.o pmtudisc.o icmpsub.o stat.o

CFLAGS	= -Wall # -g -DDEBUG


.c.o:
	gcc -c $(CFLAGS) $<

.cpp.o:
	g++ -c $(CFLAGS) $<

map646: $(OBJS)
	g++ $(CFLAGS) -o map646 $(OBJS)

clean:
	rm -f *.o map646 *~

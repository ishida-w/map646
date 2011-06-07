OBJS	= map646.o mapping.o tunif.o checksum.o pmtudisc.o icmpsub.o stat.o

CFLAGS	= -Wall -g

map646: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

.c.o:
	gcc -c $(CFLAGS) $<

.cpp.o:
	g++ -c $(CFLAGS) $<

clean:
	rm -f *.o map646 *~

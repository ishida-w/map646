OBJS	= map646.o mapping.o tunif.o checksum.o pmtudisc.o icmpsub.o stat.o

CFLAGS	= -Wall -g
LIBS = -ljson

map646: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS) $(LIBS)

stat_client:
	g++ $(CFLAGS) -o ./contrib/stat/stat_client ./contrib/stat/stat_client.cpp $(LIBS)

.c.o:
	gcc -c $(CFLAGS) $<

.cpp.o:
	g++ -c $(CFLAGS) $<

clean:
	rm -f *.o map646 *~
	rm -f ./constrib/stat/stat_clilent

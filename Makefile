OBJS	= map646.o mapping.o tunif.o checksum.o pmtudisc.o icmpsub.o stat.o

CFLAGS	= -Wall #-g -DDEBUG
LIBS = -ljson
INC = -I./include

map646: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(INC)

.c.o:
	gcc -c $(CFLAGS) $< $(INC)

.cpp.o:
	g++ -c $(CFLAGS) $< $(INC)

clean:
	rm -f *.o map646 *~

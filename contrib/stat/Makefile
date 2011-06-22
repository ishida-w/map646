OBJS = stat_client_cron.o  ../../stat.o ../../mapping.o ../../tunif.o
OBJS2 = stat_client.o

CFLAGS = -Wall -g -DDEBUG
LIBS = -ljson



stat_client: $(OBJS2)
	g++ $(CFLAGS) -o $@ $(OBJS2) $(LIBS)

stat_client_cron: $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS) $(LIBS)
.cpp.o:
	g++ -c $(CFLAGS) $<

clean:
	rm -f *.o stat_client_cron stat_client *~
